#include "booktextblock.h"
#include <QFontMetricsF>
#include <QDebug>
#include <QPainter>
#include <QElapsedTimer>

BookTextBlock::BookTextBlock(const BookTextBlockData::Ptr &data, const QSizeF &size, const BookStyle &style, const QWeakPointer<BookBlockFactory> &factory)
    : BookBlock(size, factory), m_textLayout(data->text, data->font), m_height(0), m_formats(data->formats)
{
    QTextOption textOption;
    textOption.setAlignment(Qt::AlignJustify);
    textOption.setWrapMode(QTextOption::WordWrap);

    QList<QTextLayout::FormatRange> formats;

    for (const FormatRange &range : m_formats) {
        formats.append({ range.start, range.length, style.formats.value(range.format.type) });
    }

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
    QFontMetrics fontMetrics(m_textLayout.font());
    m_height = 0;
    int leading = fontMetrics.leading();

    m_textLayout.setCacheEnabled(true);
    m_textLayout.beginLayout();
    forever {
        QTextLine line = m_textLayout.createLine();
        if (!line.isValid())
            break;

        line.setLineWidth(size.width());
        if (!qFuzzyIsNull(m_height))
            m_height += leading;
        line.setPosition(QPointF(0, 0));
        m_height += line.height();
    }
    m_textLayout.endLayout();
}
