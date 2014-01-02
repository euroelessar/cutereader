#ifndef BOOKBLOCKFACTORY_H
#define BOOKBLOCKFACTORY_H

#include "bookblock.h"

class BookItem;
class QThread;

class ItemId
{
public:
    ItemId(const QSizeF &size, const BookStyle &style);

    QSizeF size() const;
    BookStyle style() const;

    bool operator ==(const ItemId &other) const;

private:
    QSizeF m_size;
    BookStyle m_style;
    QThread *m_thread;
};

class BookBlockFactory
{
public:
    typedef QSharedPointer<BookBlockFactory> Ptr;
    typedef QWeakPointer<BookBlockFactory> WeakPtr;

    BookBlockFactory();

    BookBlock::Ptr item(const ItemId &id);

    virtual void setImageSizes(const QHash<QUrl, QSize> &imageSizes);

protected:
    virtual BookBlock::Ptr doCreate(const QSizeF &size, const BookStyle &style) = 0;

    BookBlock::Ptr findBlockNolock(const ItemId &id);
    void clearDeadBlocks();

    friend class BookBlock;
    typedef QPair<ItemId, BookBlock::WeakPtr> CachedBlock;

    QList<CachedBlock> m_cache;
    QMutex m_lock;
    WeakPtr m_pointer;
};

#endif // BOOKBLOCKFACTORY_H
