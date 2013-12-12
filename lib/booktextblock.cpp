#include "booktextblock.h"
#include <QFontMetricsF>
#include <QDebug>
#include <QPainter>

BookTextBlock::BookTextBlock(const BookTextBlockData::Ptr &data, const QSizeF &size, const QWeakPointer<BookBlockFactory> &factory)
    : BookBlock(size, factory), m_textLayout(data->text, data->font), m_height(0)
{
    QTextOption textOption;
    textOption.setAlignment(Qt::AlignJustify);
    textOption.setWrapMode(QTextOption::WordWrap);

    m_textLayout.setAdditionalFormats(data->formats);
    m_textLayout.setTextOption(textOption);

    buildLayout(size);
}

void BookTextBlock::draw(QPainter *painter, const QPointF &position, int line) const
{
    m_textLayout.lineAt(line).draw(painter, position);
}

int BookTextBlock::linesCount() const
{
    return m_textLayout.lineCount();
}

int BookTextBlock::lineForPosition(int position)
{
    return m_textLayout.lineForTextPosition(position).lineNumber();
}

BookTextBlock::LineInfo BookTextBlock::lineInfo(int lineNumber)
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
