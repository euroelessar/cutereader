#include "booktextblock.h"
#include <QFontMetricsF>
#include <QDebug>
#include <QPainter>

BookTextBlock::BookTextBlock(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats)
    : m_textLayout(text, font), m_lineWidth(-1), m_height(0)
{
    QTextOption textOption;
    textOption.setAlignment(Qt::AlignJustify);
    m_textLayout.setAdditionalFormats(formats);
    m_textLayout.setTextOption(textOption);
}

qreal BookTextBlock::height() const
{
    return m_height;
}

void BookTextBlock::setWidth(qreal lineWidth)
{
    QMutexLocker locker(&m_mutex);
    if (qFuzzyCompare(m_lineWidth, lineWidth))
        return;
    m_lineWidth = lineWidth;
    
    ensureLines();
}

qreal BookTextBlock::width() const
{
    return m_lineWidth;
}

int BookTextBlock::lineFor(int position) const
{
    QMutexLocker locker(&m_mutex);
    return m_textLayout.lineForTextPosition(position).lineNumber();
}

void BookTextBlock::draw(QPainter *painter, const QPointF &position) const
{
    QMutexLocker locker(&m_mutex);
    m_textLayout.draw(painter, position);
}

void BookTextBlock::draw(QPainter *painter, const QPointF &position, int fromPos, qreal *height) const
{
    QMutexLocker locker(&m_mutex);
    if (m_textLayout.lineCount() == 0)
        return;
    
    QTextLine firstLine = m_textLayout.lineForTextPosition(fromPos);
    qreal visibleLinesHeight = *height;
    int lastVisibleLine = m_textLayout.lineCount() - 1;
    
    checkBorders(fromPos, &visibleLinesHeight, &lastVisibleLine);
    
    if (lastVisibleLine == m_textLayout.lineCount() - 1)
        *height = visibleLinesHeight;
    
    QPointF startPosition = position - firstLine.position() + m_textLayout.lineAt(0).position();
    
    for (int i = firstLine.lineNumber(); i <= lastVisibleLine; ++i) {
        QTextLine line = m_textLayout.lineAt(i);
        line.draw(painter, startPosition);
    }
}

int BookTextBlock::lastVisiblePosition(int fromPos, qreal *height, bool *lastPosition)
{
    QMutexLocker locker(&m_mutex);
    if (m_textLayout.lineCount() == 0) {
        *lastPosition = false;
        *height = 0;
        return fromPos;
    }
    
    qreal visibleLinesHeight = *height;
    int lastVisibleLine = m_textLayout.lineCount() - 1;
    
    checkBorders(fromPos, &visibleLinesHeight, &lastVisibleLine);
    
    if (lastVisibleLine < 0) {
        *lastPosition = false;
        return 0;
    }
    
    if (lastVisibleLine == m_textLayout.lineCount() - 1) {
        *height = visibleLinesHeight;
        *lastPosition = true;
    } else {
        *lastPosition = false;
    }
    
    QTextLine lastLine = m_textLayout.lineAt(lastVisibleLine);
    return lastLine.textStart() + lastLine.textLength();
}

int BookTextBlock::inverseLastVisiblePosition(int fromPos, qreal *height, bool *lastPosition, bool *afterLastPosition)
{
    QMutexLocker locker(&m_mutex);
    *afterLastPosition = false;
    if (m_textLayout.lineCount() == 0) {
        *lastPosition = false;
        *height = 0;
        return fromPos;
    }
    
    int lineAfterLastNumber = m_textLayout.lineCount();
    int firstPos = m_textLayout.text().length() - 1;
    
    if (fromPos < m_textLayout.text().length()) {
        QTextLine lineAfterLast = m_textLayout.lineForTextPosition(fromPos);
        if (lineAfterLast.lineNumber() == 0) {
            *lastPosition = true;
            *height = 0;
            return 0;
        }
        
        lineAfterLastNumber = lineAfterLast.lineNumber();
        firstPos = m_textLayout.lineAt(lineAfterLastNumber - 1).textStart();
//        firstPos = lineAfterLast.textStart();
    }
    
    qreal visibleLinesHeight = *height;
    int lastVisibleLine = 0;
    
    inverseCheckBorders(firstPos, &visibleLinesHeight, &lastVisibleLine);
    
    if (lastVisibleLine >= m_textLayout.lineCount()) {
        *afterLastPosition = true;
        return 0;
    }
    
    if (lastVisibleLine < 0) {
        *lastPosition = false;
        return 0;
    }
    
    int textStart = 0;
    
    if (lastVisibleLine == 0) {
        *height = visibleLinesHeight;
        *lastPosition = true;
        textStart = 0;
    } else {
        *lastPosition = false;
        textStart = m_textLayout.lineAt(lastVisibleLine).textStart();
    }
    
    return textStart;
}

int BookTextBlock::linesCount() const
{
    QMutexLocker locker(&m_mutex);
    return m_textLayout.lineCount();
}

void BookTextBlock::checkBorders(int fromPos, qreal *height, int *lastVisibleLine) const
{
    QTextLine firstLine = m_textLayout.lineForTextPosition(fromPos);
    qreal startPos = firstLine.y();
    qreal maxHeight = *height;
    int i;
    for (i = firstLine.lineNumber(); i < m_textLayout.lineCount(); ++i) {
        QTextLine line = m_textLayout.lineAt(i);
        qreal tmp = line.y() + line.height() - startPos;
        if (tmp > *height) {
            break;
        } else {
            maxHeight = tmp;
        }
    }
    
    *height = maxHeight;
    if (lastVisibleLine)
        *lastVisibleLine = i - 1;
}

void BookTextBlock::inverseCheckBorders(int fromPos, qreal *height, int *lastVisibleLine) const
{
    QTextLine firstLine = m_textLayout.lineForTextPosition(fromPos);
    qreal startPos = firstLine.y() + firstLine.height();
    qreal maxHeight = *height;
    int i;
    for (i = firstLine.lineNumber(); i >= 0; --i) {
        QTextLine line = m_textLayout.lineAt(i);
        qreal tmp = startPos - line.y();
        if (tmp > *height) {
            break;
        } else {
            maxHeight = tmp;
        }
    }
    
    *height = maxHeight;
    if (lastVisibleLine)
        *lastVisibleLine = i + 1;
}

struct CharInfo
{
    const BookTextFragment *fragment;
    QChar ch;
    int position;
    qreal width;
    
    bool isSpace() const { return ch.isSpace(); }
};

void BookTextBlock::ensureLines()
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
    
        line.setLineWidth(m_lineWidth);
        m_height += leading;
        line.setPosition(QPointF(0, m_height));
        m_height += line.height();
    }
    m_textLayout.endLayout();
}
