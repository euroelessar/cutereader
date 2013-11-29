#include "bookitem.h"
#include <QXmlStreamReader>
#include <QGuiApplication>
#include <QStack>
#include <functional>
#include <QElapsedTimer>
#include <QDebug>

struct FB2FormatDescription
{
    QString name;
    std::function<void (QTextCharFormat &)> change;
};

QList<BookTextBlock::Ptr> parse_fb2(const QString &path)
{
    QFile file(path);
    file.open(QFile::ReadOnly);
    QXmlStreamReader in(&file);
    
    QList<BookTextBlock::Ptr> blocks;
    
    int depth = 0;
    
    int p_depth = -1;
    bool inP = false;
    
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
    
    int sectionsDepth = 0;
    bool inTitle = false;
    
    while (in.readNext()) {
        if (in.tokenType() == QXmlStreamReader::Invalid)
            break;
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;
            
            if (!inP && in.name() == QStringLiteral("p")) {
                inP = true;
                p_depth = depth;
            }
            if (in.name() == QStringLiteral("title")) {
                inTitle = true;
            }
            
            if (in.name() == QStringLiteral("section"))
                ++sectionsDepth;
            
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
            if (inP) {
                text += in.text();
            }
            break;
        default:
            break;
        }
    }
    
    return blocks;
}

BookItem::BookItem(QObject *parent) :
    QObject(parent)
{
}

QList<BookTextBlock::Ptr> BookItem::blocks() const
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
