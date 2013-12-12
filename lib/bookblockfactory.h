#ifndef BOOKBLOCKFACTORY_H
#define BOOKBLOCKFACTORY_H

#include "bookblock.h"

class BookBlockFactory
{
public:
    typedef QSharedPointer<BookBlockFactory> Ptr;
    typedef QWeakPointer<BookBlockFactory> WeakPtr;

    BookBlockFactory();

    BookBlock::Ptr item(const QSizeF &size);

    virtual void setImageSizes(const QHash<QUrl, QSize> &imageSizes);

protected:
    virtual BookBlock::Ptr doCreate(const QSizeF &size) = 0;

    BookBlock::Ptr findBlockNolock(const QSizeF &size);
    void clearDeadBlocks();

    friend class BookBlock;
    typedef QPair<QSizeF, BookBlock::WeakPtr> CachedBlock;

    QList<CachedBlock> m_cache;
    QMutex m_lock;
    WeakPtr m_pointer;
};

#endif // BOOKBLOCKFACTORY_H
