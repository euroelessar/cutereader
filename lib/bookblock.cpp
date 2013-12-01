#include "bookblock.h"

BookBlock::BookBlock()
{
}

BookBlock::~BookBlock()
{
}

void BookBlock::setSize(const QSizeF &size)
{
    QMutexLocker locker(&m_mutex);
    if (m_pageSize == size)
        return;
    m_pageSize = size;
    doSetSize(size);
}

qreal BookBlock::pageHeight() const
{
    return m_pageSize.height();
}

qreal BookBlock::pageWidth() const
{
    return m_pageSize.width();
}

void BookBlock::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    Q_UNUSED(imageSizes);
}
