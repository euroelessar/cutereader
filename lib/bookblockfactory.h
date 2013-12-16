#ifndef BOOKBLOCKFACTORY_H
#define BOOKBLOCKFACTORY_H

#include "bookblock.h"

class BookBlockFactory
{
public:
    typedef QSharedPointer<BookBlockFactory> Ptr;
    typedef QWeakPointer<BookBlockFactory> WeakPtr;

    BookBlockFactory();

    BookBlock::Ptr item(const QSizeF &size, const BookStyle &style);

    virtual void setImageSizes(const QHash<QUrl, QSize> &imageSizes);

protected:
    virtual BookBlock::Ptr doCreate(const QSizeF &size, const BookStyle &style) = 0;

    BookBlock::Ptr findBlockNolock(const QSizeF &size, const BookStyle &style);
    void clearDeadBlocks();

    friend class BookBlock;
    typedef QPair<QPair<QSizeF, int>, BookBlock::WeakPtr> CachedBlock;

    QList<CachedBlock> m_cache;
    QMutex m_lock;
    WeakPtr m_pointer;
};

#endif // BOOKBLOCKFACTORY_H
