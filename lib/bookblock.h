#ifndef BOOKBLOCK_H
#define BOOKBLOCK_H

#include <QSharedPointer>
#include <QMutex>
#include <QPainter>
#include <QQuickItem>

class BookBlock
{
public:
    typedef QSharedPointer<BookBlock> Ptr;
    
    BookBlock();
    virtual ~BookBlock();
    BookBlock(const BookBlock &other) = delete;
    BookBlock &operator =(const BookBlock &other) = delete;
    
    
    virtual qreal height() const = 0;
    void setSize(const QSizeF &size);
    qreal pageHeight() const;
    qreal pageWidth() const;
    
    struct ItemInfo {
        QString type;
        QVariantMap properties;
    };
    
    virtual void draw(QPainter *painter, const QPointF &position, int fromPos, qreal *height) const = 0;
    virtual QList<ItemInfo> createItems(const QPointF &position, int fromPos, qreal *height) const = 0;
    
    virtual int lastVisiblePosition(int fromPos, qreal *height, bool *lastPosition) = 0;
    virtual int inverseLastVisiblePosition(int fromPos, qreal *height, bool *lastPosition, bool *afterLastPosition) = 0;
    virtual void setImageSizes(const QHash<QUrl, QSize> &imageSizes);
    
protected:
    virtual void doSetSize(const QSizeF &size) = 0;
    
    mutable QMutex m_mutex;
    
private:
    QSizeF m_pageSize;
};

Q_DECLARE_METATYPE(QList<BookBlock::ItemInfo>)

#endif // BOOKBLOCK_H
