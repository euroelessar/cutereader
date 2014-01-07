#include "booktextblock.h"
#include "../bookstyle.h"
#include <QFontMetricsF>
#include <QDebug>
#include <QPainter>
#include <QElapsedTimer>
#include <QDebug>

BookTextBlock::BookTextBlock(const BookTextBlockData::Ptr &data, const QSizeF &size, const BookStyle &style, const QWeakPointer<BookBlockFactory> &factory)
    : BookBlock(size, factory), m_textLayout(data->text), m_formats(data->formats)
{
    QList<QTextLayout::FormatRange> formats;

    for (const FormatRange &range : m_formats) {
        QTextCharFormat format = style.formats[range.format.type];

        int index = BookStyleItem::indexForFormat(range.format.type);
        if (index >= 0 && index < style.colors.size() && style.colors[index].isValid())
            format.setForeground(style.colors[index]);

        formats.append({ range.start, range.length, format });

        if (range.start == 0 && range.length == data->text.size())
            m_blockFormat.merge(style.blockFormats[range.format.type]);
    }

    QTextOption textOption;
    textOption.setAlignment(m_blockFormat.alignment());
    textOption.setWrapMode(QTextOption::WordWrap);

    m_textLayout.setAdditionalFormats(formats);
    m_textLayout.setTextOption(textOption);

    buildLayout(size);
}

void BookTextBlock::draw(QPainter *painter, const QPointF &position, int line) const
{
    m_textLayout.lineAt(line).draw(painter, position);
}

QList<BookBlock::ItemInfo> BookTextBlock::createItems(const QPointF &position, int line) const
{
    QList<ItemInfo> result;
    const QTextLine textLine = m_textLayout.lineAt(line);
    const int begin = textLine.textStart();
    const int end = begin + textLine.textLength();

    for (const FormatRange &range : m_formats) {
        const int rangeBegin = qMax(begin, range.start);
        const int rangeEnd = qMin(end, range.start + range.length);
        if (rangeBegin >= rangeEnd || range.format.href.isEmpty())
            continue;

        const qreal xBegin = position.x() + textLine.cursorToX(rangeBegin);
        const qreal xEnd = position.x() + textLine.cursorToX(rangeEnd);
        const qreal delta = textLine.height() / 2;

        ItemInfo info = {
            QStringLiteral("link"),
            {
                { "x", xBegin - delta },
                { "y", position.y() - delta },
                { "width", xEnd - xBegin + 2 * delta },
                { "height", textLine.height() + 2 * delta },
                { "href", range.format.href }
            }
        };

        result << info;
    }

    return result;
}

int BookTextBlock::linesCount() const
{
    return m_textLayout.lineCount();
}

int BookTextBlock::lineForPosition(int position) const
{
    return m_textLayout.lineForTextPosition(position).lineNumber();
}

BookTextBlock::LineInfo BookTextBlock::lineInfo(int lineNumber) const
{
    QTextLine line = m_textLayout.lineAt(lineNumber);
    return {
        line.height(),
        line.textStart(),
        line.textLength()
    };
}

void BookTextBlock::buildLayout(const QSizeF &size)
{
    const QString text = m_textLayout.text();

    QFontMetrics fontMetrics(m_textLayout.font());
    qreal height = 0;
    int leading = fontMetrics.leading();

    m_textLayout.setCacheEnabled(true);
    m_textLayout.beginLayout();
    forever {
        QTextLine line = m_textLayout.createLine();
        if (!line.isValid())
            break;

        const bool firstLine = line.textStart() == 0;
        const qreal leftMargin = (firstLine ? m_blockFormat.textIndent() : 0)
                + m_blockFormat.leftMargin();
        const qreal rightMargin = m_blockFormat.rightMargin();

        line.setLineWidth(size.width() - leftMargin - rightMargin);
        if (line.textLength() > 0) {
            int lastCharIndex = line.textStart() + line.textLength() - 1;
            QChar lastChar = text.at(lastCharIndex);
            if (lastChar == QChar::SoftHyphen)
                line.setLineWidth(line.width() - fontMetrics.width(QChar::SoftHyphen));
        }

        if (!qFuzzyIsNull(height))
            height += leading;
        line.setPosition(QPointF(leftMargin, 0));
        height += line.height();
    }
    m_textLayout.endLayout();
}
