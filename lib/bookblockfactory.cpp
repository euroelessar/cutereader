#include "bookblockfactory.h"

BookBlockFactory::BookBlockFactory()
{
}

static bool isEqual(const QSizeF &first, const QSizeF &second)
{
    return qFuzzyCompare(first.width(), second.width())
            && qFuzzyCompare(first.height(), second.height());
}

BookBlock::Ptr BookBlockFactory::item(const QSizeF &size, const BookStyle &style)
{
    {
        QMutexLocker locker(&m_lock);
        if (auto block = findBlockNolock(size, style))
            return block;
    }

    auto block = doCreate(size, style);
    return block;

    QMutexLocker locker(&m_lock);

    if (auto cachedBlock = findBlockNolock(size, style))
        return cachedBlock;

    m_cache << qMakePair(qMakePair(size, style.generation), block.toWeakRef());
    return block;
}

void BookBlockFactory::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    Q_UNUSED(imageSizes);
}

BookBlock::Ptr BookBlockFactory::findBlockNolock(const QSizeF &size, const BookStyle &style)
{
    QListIterator<CachedBlock> it(m_cache);

    while (it.hasNext()) {
        const auto &pair = it.next();
        if (isEqual(pair.first.first, size) && pair.first.second == style.generation) {
            if (auto block = pair.second.toStrongRef())
                return block;
        }
    }

    return BookBlock::Ptr();
}

void BookBlockFactory::clearDeadBlocks()
{
    QMutexLocker locker(&m_lock);
    QMutableListIterator<CachedBlock> it(m_cache);

    while (it.hasNext()) {
        if (it.next().second.isNull())
            it.remove();
    }
}
