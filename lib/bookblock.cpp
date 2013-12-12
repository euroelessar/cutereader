#include "bookblock.h"
#include "bookblockfactory.h"

BookBlock::BookBlock(const QSizeF &size, const QWeakPointer<BookBlockFactory> &factory)
    : m_pageSize(size), m_factory(factory)
{
    Q_ASSERT(m_factory);
}

BookBlock::~BookBlock()
{
    if (auto factory = m_factory.toStrongRef())
        factory->clearDeadBlocks();
}

qreal BookBlock::pageHeight() const
{
    return m_pageSize.height();
}

qreal BookBlock::pageWidth() const
{
    return m_pageSize.width();
}

QList<BookBlock::ItemInfo> BookBlock::createItems(const QPointF &position, int line) const
{
    Q_UNUSED(position);
    Q_UNUSED(line);
    return QList<BookBlock::ItemInfo>();
}

void BookBlock::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    Q_UNUSED(imageSizes);
}
