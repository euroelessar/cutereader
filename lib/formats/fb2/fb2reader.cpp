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

FB2Reader::FB2Reader(const QUrl &source) : m_source(source)
{
}

struct FB2FormatDescription
{
    QString name;
    std::function<void (QTextCharFormat &)> change;
};

QList<BookBlock::Ptr> FB2Reader::read(QIODevice *device)
{
    QXmlStreamReader in(device);
    
    QList<BookBlock::Ptr> blocks;
    
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
    
    QString binaryId;
    
    while (in.readNext()) {
        if (in.tokenType() == QXmlStreamReader::Invalid)
            break;
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;
            
            if (in.name() == QStringLiteral("binary")) {
                inBinary = true;
                binaryId = in.attributes().value(QStringLiteral("id")).toString();
            }
            
            if (!inP && in.name() == QStringLiteral("p")) {
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
                QString href = in.attributes().value(xlink, QStringLiteral("href")).toString();
                blocks << BookImageBlock::create(m_source.resolved(QUrl(href)));
            }
            
            if (inP) {
                if (find_description(in.name())) {
                    changes << text.size();
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if (inP && p_depth == depth) {
                if (inTitle) {
                    QTextCharFormat format;
                    format.setFontWeight(QFont::Bold);
                    QTextLayout::FormatRange range = { 0, text.size(), format };
                    formats.append(range);
                }
                blocks << BookTextBlock::create(text, qApp->font(), formats);
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
                QUrl url(m_source);
                url.setFragment(binaryId);
                imageSizes.insert(url, reader.size());
            }
            if (inP) {
                text += in.text();
            }
            break;
        default:
            break;
        }
    }
    
    for (BookBlock::Ptr block : blocks)
        block->setImageSizes(imageSizes);
    
    return blocks;
}
