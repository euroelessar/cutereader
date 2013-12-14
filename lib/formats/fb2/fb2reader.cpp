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
#include <functional>

FB2Reader::FB2Reader()
{
    using namespace std::placeholders;

    m_descriptions = {
        {
            QStringLiteral("strong"),
            std::bind(&QTextCharFormat::setFontWeight, _1, QFont::Bold)
        },
        {
            QStringLiteral("emphasis"),
            std::bind(&QTextCharFormat::setFontItalic, _1, true)
        },
        {
            QStringLiteral("strikethrough"),
            std::bind(&QTextCharFormat::setFontStrikeOut, _1, true)
        },
        {
            QStringLiteral("sub"),
            std::bind(&QTextCharFormat::setVerticalAlignment, _1, QTextCharFormat::AlignSubScript)
        },
        {
            QStringLiteral("sup"),
            std::bind(&QTextCharFormat::setFontItalic, _1, QTextCharFormat::AlignSuperScript)
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
                    inAuthor = true;
                }
            } else if (depth == 4 && inCoverPage && in.name() == QStringLiteral("image")) {
                info.cover = readImage(in, baseUrl).source;
                --depth;
            } else if (depth == 4 && inAuthor) {
                QString text = in.readElementText(QXmlStreamReader::IncludeChildElements);
                --depth;

                if (in.name() == QStringLiteral("first-name"))
                    info.author.firstName = text;
                else if (in.name() == QStringLiteral("last-name"))
                    info.author.lastName = text;
                else if (in.name() == QStringLiteral("middle-name"))
                    info.author.middleName = text;
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

BookBlockFactory::Ptr FB2Reader::readParagraph(QXmlStreamReader &in, const QList<QTextCharFormat> &baseFormats)
{
    Q_ASSERT(in.name() == QStringLiteral("p"));

    int depth = 1;
    QString text;
    QList<QTextLayout::FormatRange> formats;
    QStack<int> changes;

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

            if (find_description(in.name())) {
                changes << text.size();
            }

            break;
        case QXmlStreamReader::EndElement:
            --depth;
            if (depth == 0) {
                Q_ASSERT(in.name() == QStringLiteral("p"));
                for (int i = baseFormats.size() - 1; i >= 0; --i)
                    formats.append({ 0, text.size(), baseFormats[i] });

                return BookTextBlockFactory::create(text, qApp->font(), formats);
            }

            if (FormatDescription *description = find_description(in.name())) {
                QTextCharFormat format;
                description->change(format);

                auto position = changes.takeLast();
                formats.append({ position, text.size() - position, format });
            }
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

QList<BookBlockFactory::Ptr> FB2Reader::readBody(QXmlStreamReader &in, const QUrl &baseUrl)
{
    Q_ASSERT(in.name() == QStringLiteral("body") || in.name() == QStringLiteral("annotation"));

    int depth = 1;
    int sectionsDepth = 0;
    int inTitleCounter = 0;
    QList<BookBlockFactory::Ptr> blocks;

    QTextCharFormat titleFormat;
    titleFormat.setFontWeight(QFont::Bold);

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (in.name() == QStringLiteral("section")) {
                ++sectionsDepth;
            } else if (in.name() == QStringLiteral("title")) {
                ++inTitleCounter;
            } else if (in.name() == QStringLiteral("p")) {
                QList<QTextCharFormat> baseFormats;
                if (inTitleCounter)
                    baseFormats << titleFormat;

                blocks << readParagraph(in, baseFormats);
                --depth;
            } else if (in.name() == QStringLiteral("image")) {
                blocks << readImage(in, baseUrl).toFactory();
                --depth;
            }

            break;
        case QXmlStreamReader::EndElement:
            --depth;
            if (depth == 0) {
                return blocks;
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
    return QList<BookBlockFactory::Ptr>();
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

    int depth = 0;
    bool inBinary = false;

    QString binaryId;

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (in.name() == QStringLiteral("binary") && (flags & ImageSizes)) {
                inBinary = true;
                binaryId = in.attributes().value(QStringLiteral("id")).toString();
            }

            if (in.name() == QStringLiteral("body")) {
                if (flags & Text)
                    result.blocks << readBody(in, baseUrl);
                else
                    in.skipCurrentElement();
            } else if (in.name() == QStringLiteral("description")) {
                readDescription(in, result, baseUrl);
            }
            break;
        case QXmlStreamReader::EndElement:
            if (in.name() == QStringLiteral("binary")) {
                inBinary = false;
            }

            --depth;
            break;
        case QXmlStreamReader::Characters:
            if (inBinary) {
                QByteArray binary = QByteArray::fromBase64(in.text().toLatin1());
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

    for (BookBlockFactory::Ptr block : result.blocks + result.annotation)
        block->setImageSizes(imageSizes);

    return result;
}

BookBlockFactory::Ptr FB2Reader::ImageInfo::toFactory()
{
    return BookImageBlockFactory::create(source);
}
