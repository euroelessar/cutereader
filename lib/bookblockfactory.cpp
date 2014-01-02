#include "bookblockfactory.h"
#include "bookitem.h"
#include <QThread>

ItemId::ItemId(const QSizeF &size, const BookStyle &style) :
    m_size(size),
    m_style(style),
    m_thread(QThread::currentThread())
{
}

QSizeF ItemId::size() const
{
    return m_size;
}

BookStyle ItemId::style() const
{
    return m_style;
}

static bool isEqual(const QSizeF &first, const QSizeF &second)
{
    return qFuzzyCompare(first.width(), second.width())
            && qFuzzyCompare(first.height(), second.height());
}

bool ItemId::operator ==(const ItemId &other) const
{
    return m_style.generation == other.m_style.generation
            && m_thread == other.m_thread
            && isEqual(m_size, other.m_size);
}

BookBlockFactory::BookBlockFactory()
{
}

BookBlock::Ptr BookBlockFactory::item(const ItemId &id)
{
    {
        QMutexLocker locker(&m_lock);
        if (auto block = findBlockNolock(id))
            return block;
    }

    auto block = doCreate(id.size(), id.style());

    QMutexLocker locker(&m_lock);

    if (auto cachedBlock = findBlockNolock(id))
        return cachedBlock;

    m_cache << qMakePair(id, block.toWeakRef());
    return block;
}

void BookBlockFactory::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    Q_UNUSED(imageSizes);
}

BookBlock::Ptr BookBlockFactory::findBlockNolock(const ItemId &id)
{
    QListIterator<CachedBlock> it(m_cache);

    while (it.hasNext()) {
        const auto &pair = it.next();
        if (pair.first == id) {
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
