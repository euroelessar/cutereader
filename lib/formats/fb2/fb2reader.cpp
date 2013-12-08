#include "fb2reader.h"
#include "../lib/bookimageblock.h"
#include "../lib/booktextblock.h"

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
}

bool FB2Reader::canRead(const QMimeType &mimeType) const
{
    return mimeType.inherits(QStringLiteral("application/x-fictionbook"));
}

struct FB2FormatDescription
{
    QString name;
    std::function<void (QTextCharFormat &)> change;
};

BookInfo FB2Reader::read(const QUrl &source, QIODevice *device, Flags flags)
{
    BookInfo result;
    result.source = source;

    const QUrl baseUrl(QStringLiteral("image://fb2/") + source.toLocalFile().toUtf8().toHex());
    QXmlStreamReader in(device);

    QList<BookBlock::Ptr> *blocks = NULL;

    FB2FormatDescription descriptions[] = {
        {
            QStringLiteral("strong"),
            [] (QTextCharFormat &format) { format.setFontWeight(QFont::Bold); }
        },
        {
            QStringLiteral("emphasis"),
            [] (QTextCharFormat &format) { format.setFontItalic(true); }
        }
    };

    auto find_description = [&descriptions] (const QStringRef &name) -> FB2FormatDescription * {
        for (auto &description : descriptions) {
            if (description.name == name)
                return &description;
        }
        return NULL;
    };

    QString text;
    QTextCharFormat format;
    format.setFont(qApp->font());
    QList<QTextLayout::FormatRange> formats;
    QStack<int> changes;

    QHash<QUrl, QSize> imageSizes;

    int depth = 0;
    int sectionsDepth = 0;
    bool inP = false;
    int p_depth = -1;
    bool inTitle = false;
    bool inBinary = false;
    bool inTitleInfo = false;

    QString binaryId;
    QString currentTitleInfo;

    while (in.readNext()) {
        if (in.tokenType() == QXmlStreamReader::Invalid)
            break;
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (in.name() == QStringLiteral("binary") && (flags & ImageSizes)) {
                inBinary = true;
                binaryId = in.attributes().value(QStringLiteral("id")).toString();
            }

            if (inTitleInfo && in.name() == QStringLiteral("annotation") && (flags & Info)) {
                blocks = &result.annotation;
            }

            if (in.name() == QStringLiteral("body") && (flags & Text)) {
                blocks = &result.blocks;
                const auto &attributes = in.attributes();
                if (!attributes.isEmpty())
                    qDebug() << attributes.first().name() << attributes.first().value();
            }

            if (blocks && !inP && in.name() == QStringLiteral("p")) {
                inP = true;
                p_depth = depth;
            }
            if (in.name() == QStringLiteral("title")) {
                inTitle = true;
            }

            if (in.name() == QStringLiteral("section"))
                ++sectionsDepth;

            if (in.name() == QStringLiteral("image")) {
                const QString xlink = QStringLiteral("http://www.w3.org/1999/xlink");
                const QString href = in.attributes().value(xlink, QStringLiteral("href")).toString();
                const QUrl url = baseUrl.resolved(QUrl(href));

                if (blocks) {
                    (*blocks) << BookImageBlock::create(url);
                } else if (currentTitleInfo == QStringLiteral("coverpage")) {
                    result.cover = url;
                }
            }

            if (in.name() == QStringLiteral("title-info") && (flags & Info)) {
                inTitleInfo = true;
            } else if (inTitleInfo) {
                currentTitleInfo = in.name().toString();
            }

            if (inP) {
                if (find_description(in.name())) {
                    changes << text.size();
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if (inP && p_depth == depth) {
                Q_ASSERT(blocks);
                if (inTitle) {
                    QTextCharFormat format;
                    format.setFontWeight(QFont::Bold);
                    QTextLayout::FormatRange range = { 0, text.size(), format };
                    formats.append(range);
                }
                (*blocks) << BookTextBlock::create(text, qApp->font(), formats);
                text.clear();
                formats.clear();
                inP = false;
            }
            if (in.name() == QStringLiteral("section"))
                --sectionsDepth;
            if (in.name() == QStringLiteral("title")) {
                inTitle = false;
            }
            if (in.name() == QStringLiteral("binary")) {
                inBinary = false;
            }
            if (in.name() == QStringLiteral("title-info")) {
                inTitleInfo = false;
            }
            if (in.name() == QStringLiteral("annotation")) {
                blocks = NULL;
            }
            if (in.name() == QStringLiteral("body")) {
                blocks = NULL;
            }
            currentTitleInfo.clear();

            if (inP) {
                if (FB2FormatDescription *description = find_description(in.name())) {
                    QTextCharFormat format;
                    description->change(format);

                    auto position = changes.takeLast();
                    QTextLayout::FormatRange range = { position, text.size() - position, format };
                    formats << range;
                }
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
            if (inP) {
                text += in.text();
            }
            if (inTitleInfo && currentTitleInfo == QStringLiteral("book-title")) {
                result.title = in.text().toString();
            }
            break;
        default:
            break;
        }
    }

    for (BookBlock::Ptr block : result.blocks + result.annotation)
        block->setImageSizes(imageSizes);

    return result;
}
