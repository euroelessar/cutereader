#ifndef BOOKTEXTBLOCK_H
#define BOOKTEXTBLOCK_H

#include "booktextfragment.h"
#include <QStringList>
#include <QTextLayout>
#include <QSharedPointer>
#include <QMutex>

class BookTextBlock
{
public:
    typedef QSharedPointer<BookTextBlock> Ptr;
    
    BookTextBlock(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats);
    BookTextBlock(const BookTextBlock &other) = delete;
    BookTextBlock &operator =(const BookTextBlock &other) = delete;
    
    template <typename... Args>
    static Ptr create(Args &&...args)
    {
        return Ptr::create(std::forward<Args>(args)...);
    }
    
    qreal height() const;
    void setWidth(qreal widht);
    qreal width() const;
    
    int lineFor(int position) const;
    
    void draw(QPainter *painter, const QPointF &position) const;
    void draw(QPainter *painter, const QPointF &position, int fromPos, qreal *height) const;
    
    int lastVisiblePosition(int fromPos, qreal *height, bool *lastPosition);
    int inverseLastVisiblePosition(int fromPos, qreal *height, bool *lastPosition, bool *afterLastPosition);
    
    int linesCount() const;
    
private:
    void checkBorders(int fromPos, qreal *height, int *lastVisibleLine) const;
    void inverseCheckBorders(int fromPos, qreal *height, int *lastVisibleLine) const;
    void ensureLines();
    
    QTextLayout m_textLayout;
    mutable QMutex m_mutex;

    qreal m_lineWidth;
    qreal m_height;
//    mutable QList<QList<BookTextFragment>> m_lines;
};

#endif // BOOKTEXTBLOCK_H
