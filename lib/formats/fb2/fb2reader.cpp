#include "fb2reader.h"
#include "../lib/blocks/bookimageblockfactory.h"
#include "../lib/blocks/booktextblockfactory.h"

#include <QXmlStreamReader>
#include <QTextLayout>
#include <QGuiApplication>
#include <QImageReader>
#include <QBuffer>
#include <QTextCharFormat>
#include <QStack>
#include <QElapsedTimer>
#include <functional>

#ifndef Q_COMPILER_LAMBDA
# error Lambda support is required
#endif

FB2Reader::FB2Reader()
{
    using namespace std::placeholders;

    m_descriptions = {
        {
            QStringLiteral("strong"),
            [] (const QXmlStreamReader &, Format &format) { format.type = Format::Strong; }
        },
        {
            QStringLiteral("emphasis"),
            [] (const QXmlStreamReader &, Format &format) { format.type = Format::Emphasis; }
        },
        {
            QStringLiteral("strikethrough"),
            [] (const QXmlStreamReader &, Format &format) { format.type = Format::StrikeThrough; }
        },
        {
            QStringLiteral("sub"),
            [] (const QXmlStreamReader &, Format &format) { format.type = Format::Sub; }
        },
        {
            QStringLiteral("sup"),
            [] (const QXmlStreamReader &, Format &format) { format.type = Format::Sup; }
        },
        {
            QStringLiteral("a"),
            [] (const QXmlStreamReader &in, Format &format) {
                const QString xlink = QStringLiteral("http://www.w3.org/1999/xlink");
                const QStringRef type = in.attributes().value(QStringLiteral("type"));
                const QStringRef href = in.attributes().value(xlink, QStringLiteral("href"));

                if (href.isEmpty())
                    return;

                if (type == QStringLiteral("note"))
                    format.type = Format::NoteAnchor;
                else if (href.startsWith(QLatin1Char('#')))
                    format.type = Format::InternalAnchor;
                else
                    format.type = Format::ExternalAnchor;

                format.href = href.toString();
            }
        }
    };
}

bool FB2Reader::canRead(const QMimeType &mimeType) const
{
    return mimeType.inherits(QStringLiteral("application/x-fictionbook"));
}

void FB2Reader::readDescription(QXmlStreamReader &in, BookInfo &info, const QUrl &baseUrl)
{
    int depth = 1;
    bool inCoverPage = false;
    bool inAuthor = false;

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (depth == 2 && in.name() != QStringLiteral("title-info")) {
                in.skipCurrentElement();
                --depth;
            } else if (depth == 3) {
                if (in.name() == QStringLiteral("annotation")) {
                    info.annotation = readBody(in, baseUrl);
                    --depth;
                } else if (in.name() == QStringLiteral("book-title")) {
                    info.title = in.readElementText();
                    --depth;
                } else if (in.name() == QStringLiteral("genre")) {
                    info.genres << in.readElementText();
                    --depth;
                } else if (in.name() == QStringLiteral("coverpage")) {
                    inCoverPage = true;
                } else if (in.name() == QStringLiteral("author")) {
                    info.authors.append(AuthorInfo());
                    inAuthor = true;
                } else if (in.name() == QStringLiteral("sequence")) {
                    QStringRef name = in.attributes().value(QStringLiteral("name"));
                    QStringRef numberStr = in.attributes().value(QStringLiteral("number"));
                    bool ok = false;
                    int number = numberStr.toInt(&ok);
                    info.sequences.append({ name.toString(), ok ? number : -1 });
                }
            } else if (depth == 4 && inCoverPage && in.name() == QStringLiteral("image")) {
                info.cover = readImage(in, baseUrl).source;
                --depth;
            } else if (depth == 4 && inAuthor) {
                QString text = in.readElementText(QXmlStreamReader::IncludeChildElements);
                --depth;

                if (in.name() == QStringLiteral("first-name"))
                    info.authors.last().firstName = text;
                else if (in.name() == QStringLiteral("last-name"))
                    info.authors.last().lastName = text;
                else if (in.name() == QStringLiteral("middle-name"))
                    info.authors.last().middleName = text;
            }

            break;
        case QXmlStreamReader::EndElement:
            if (depth == 3 && inCoverPage)
                inCoverPage = false;
            else if (depth == 3 && inAuthor)
                inAuthor = false;

            if (depth == 1)
                return;
            --depth;
            break;
        case QXmlStreamReader::Characters:
            break;
        default:
            break;
        }
    }

    Q_ASSERT(!"Impossible situation");
}

BookBlockFactory::Ptr FB2Reader::readParagraph(QXmlStreamReader &in, const QList<Format> &baseFormats)
{
    Q_ASSERT(in.name() == QStringLiteral("p"));

    int depth = 1;
    QString text;
    QList<FormatRange> formats;

    struct ChangeData
    {
        int start;
        int depth;
        Format format;
    };
    QStack<ChangeData> changes;

    auto find_description = [this] (const QStringRef &name) -> FormatDescription * {
        for (auto &description : m_descriptions) {
            if (description.name == name)
                return &description;
        }
        return NULL;
    };

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (FormatDescription *description = find_description(in.name())) {
                Format format;
                description->change(in, format);

                changes.append({ text.size(), depth, format });
            }

            break;
        case QXmlStreamReader::EndElement:
            if (depth == 1) {
                Q_ASSERT(in.name() == QStringLiteral("p"));
                for (int i = baseFormats.size() - 1; i >= 0; --i)
                    formats.prepend({ 0, text.size(), baseFormats[i] });

                return BookTextBlockFactory::create(text, formats);
            }

            if (!changes.isEmpty() && changes.last().depth == depth) {
                auto change = changes.takeLast();
                formats.append({ change.start, text.size() - change.start, change.format });
            }
            --depth;
            break;
        case QXmlStreamReader::Characters:
            text += in.text();
            break;
        default:
            break;
        }
    }

    Q_ASSERT(!"Impossible situation");
    return BookBlockFactory::Ptr();
}

FB2Reader::ImageInfo FB2Reader::readImage(QXmlStreamReader &in, const QUrl &baseUrl)
{
    Q_ASSERT(in.name() == QStringLiteral("image"));

    const QString xlink = QStringLiteral("http://www.w3.org/1999/xlink");
    const QString href = in.attributes().value(xlink, QStringLiteral("href")).toString();
    const QUrl source = baseUrl.resolved(QUrl(href));

    in.skipCurrentElement();

    return {
        source
    };
}

BodyInfo FB2Reader::readBody(QXmlStreamReader &in, const QUrl &baseUrl)
{
    Q_ASSERT(in.name() == QStringLiteral("body") || in.name() == QStringLiteral("annotation"));
    BodyInfo info;

    if (in.name() == QStringLiteral("body")) {
        QStringRef name = in.attributes().value(QStringLiteral("name"));
        if (!name.isEmpty())
            info.name = name.toString();
    }

    int depth = 1;
    int sectionsDepth = 0;
    int inTitleCounter = 0;

    const QList<Format> titleFormat = { { Format::Base }, { Format::Title } };
    const QList<Format> standardFormat = { { Format::Base }, { Format::Standard } };

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (in.name() == QStringLiteral("section")) {
                QStringRef id = in.attributes().value(QStringLiteral("id"));
                if (!id.isEmpty())
                    info.references.insert(id.toString(), { info.blocks.size(), 0 });

                ++sectionsDepth;
            } else if (in.name() == QStringLiteral("title")) {
                ++inTitleCounter;
            } else if (in.name() == QStringLiteral("p")) {
                QList<Format> baseFormats;
                if (inTitleCounter)
                    baseFormats = titleFormat;
                else
                    baseFormats = standardFormat;

                info.blocks << readParagraph(in, baseFormats);
                --depth;
            } else if (in.name() == QStringLiteral("image")) {
                info.blocks << readImage(in, baseUrl).toFactory();
                --depth;
            }

            break;
        case QXmlStreamReader::EndElement:
            --depth;
            if (depth == 0) {
                return info;
            }

            if (in.name() == QStringLiteral("section"))
                --sectionsDepth;
            else if (in.name() == QStringLiteral("title"))
                --inTitleCounter;
            break;
        case QXmlStreamReader::Characters:
            break;
        default:
            break;
        }
    }

    Q_ASSERT(!"Impossible situation");
    return info;
}

BookInfo FB2Reader::read(const QUrl &source, QIODevice *device, Flags flags)
{
    BookInfo result;
    result.source = source;

    const QUrl baseUrl(QStringLiteral("image://fb2/") + source.toLocalFile().toUtf8().toHex());
    QXmlStreamReader in(device);

    QHash<QUrl, QSize> imageSizes;

    QTextCharFormat titleFormat;
    titleFormat.setFontWeight(QFont::Bold);

    bool inBinary = false;

    QString binaryId;

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            if (in.name() == QStringLiteral("binary") && (flags & ImageSizes)) {
                inBinary = true;
                binaryId = in.attributes().value(QStringLiteral("id")).toString();
            } else if (in.name() == QStringLiteral("body") && (flags & Text)) {
                result.bodies << readBody(in, baseUrl);
            } else if (in.name() == QStringLiteral("description") && (flags & Info)) {
                readDescription(in, result, baseUrl);
            } else if (in.name() != QStringLiteral("FictionBook")) {
                in.skipCurrentElement();
            }
            break;
        case QXmlStreamReader::EndElement:
            if (in.name() == QStringLiteral("binary"))
                inBinary = false;
            break;
        case QXmlStreamReader::Characters:
            if (inBinary) {
                QByteArray latinString = in.text().toLatin1();
                QByteArray binary = QByteArray::fromBase64(latinString);

                QBuffer buffer(&binary);
                buffer.open(QIODevice::ReadOnly);
                QImageReader reader(&buffer);
                QUrl url(baseUrl);
                url.setFragment(binaryId);
                imageSizes.insert(url, reader.size());
            }
            break;
        default:
            break;
        }
    }

    for (auto &body : result.bodies) {
        for (auto &block : body.blocks)
            block->setImageSizes(imageSizes);
    }
    for (auto &block : result.annotation.blocks)
        block->setImageSizes(imageSizes);

    return result;
}

BookBlockFactory::Ptr FB2Reader::ImageInfo::toFactory()
{
    return BookImageBlockFactory::create(source);
}
