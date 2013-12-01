#include "bookitem.h"
#include <QXmlStreamReader>
#include <QGuiApplication>
#include <QStack>
#include <functional>
#include <QElapsedTimer>
#include <QDebug>
#include "bookpageitem.h"
#include "bookimageblock.h"
#include <QImageReader>
#include <QBuffer>
#include "fb2imageprovider.h"

struct FB2FormatDescription
{
    QString name;
    std::function<void (QTextCharFormat &)> change;
};

QList<BookBlock::Ptr> parse_fb2(const QString &path)
{
    const QUrl baseUrl = [&path] () {
        QUrl url = QUrl::fromLocalFile(path);
        url.setScheme(QStringLiteral("image"));
        url.setHost(QStringLiteral("fb2"));
        return url;
    }();
    QFile file(path);
    file.open(QFile::ReadOnly);
    QXmlStreamReader in(&file);
    
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
                blocks << BookImageBlock::create(baseUrl.resolved(QUrl(href)));
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
                QUrl url(baseUrl);
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

BookItem::BookItem(QObject *parent) :
    QObject(parent)
{
}

QList<BookBlock::Ptr> BookItem::blocks() const
{
    return m_blocks;
}

void BookItem::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_source = source;

        if (source.isLocalFile()) {
            QElapsedTimer timer;
            timer.start();
            m_blocks = parse_fb2(source.toLocalFile());
            qDebug() << timer.restart();
        }
        emit sourceChanged(source);
    }
}

QUrl BookItem::source() const
{
    return m_source;
}

void BookItem::registerQmlTypes(QQmlEngine *engine)
{
    engine->addImageProvider("fb2", new FB2ImageProvider);
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BookPage");
}
