#ifndef BOOKTEXTBLOCK_H
#define BOOKTEXTBLOCK_H

#include "bookblock.h"
#include <QStringList>
#include <QTextLayout>
#include <QSharedPointer>
#include <QMutex>

class BookTextBlock : public BookBlock
{
public:
    typedef QSharedPointer<BookTextBlock> Ptr;
    
    BookTextBlock(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats);
    
    template <typename... Args>
    static Ptr create(Args &&...args)
    {
        return Ptr::create(std::forward<Args>(args)...);
    }
    
    qreal height() const;
    
    int lineFor(int position) const;
    
    void draw(QPainter *painter, const QPointF &position, int fromPos, qreal *height) const;
    QList<ItemInfo> createItems(const QPointF &position, int fromPos, qreal *height) const;
    
    int lastVisiblePosition(int fromPos, qreal *height, bool *lastPosition);
    int inverseLastVisiblePosition(int fromPos, qreal *height, bool *lastPosition, bool *afterLastPosition);
    
    int linesCount() const;
    
private:
    void checkBorders(int fromPos, qreal *height, int *lastVisibleLine) const;
    void inverseCheckBorders(int fromPos, qreal *height, int *lastVisibleLine) const;
    void doSetSize(const QSizeF &size);
    
    QTextLayout m_textLayout;

    qreal m_height;
};

#endif // BOOKTEXTBLOCK_H
