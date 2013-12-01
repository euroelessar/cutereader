#include "bookimageblock.h"
#include <QQuickView>
#include <QDebug>

BookImageBlock::BookImageBlock(const QUrl &source) : m_source(source), m_imageSize(0, 0)
{
}

qreal BookImageBlock::height() const
{
    return m_imageSize.height();
}

void BookImageBlock::draw(QPainter *painter, const QPointF &position, int fromPos, qreal *height) const
{
    Q_UNUSED(painter);
    Q_UNUSED(position);
    Q_UNUSED(fromPos);
    if (isEnoughtHeight(*height)) {
        *height = m_imageSize.height();
    }
}

QList<BookBlock::ItemInfo> BookImageBlock::createItems(const QPointF &position, int fromPos, qreal *height) const
{
    Q_UNUSED(position);
    Q_UNUSED(fromPos);
    
    if (isEnoughtHeight(*height)) {
        *height = m_imageSize.height();
        
        if (m_size.isValid()) {
            ItemInfo info = {
                QStringLiteral("image"),
                {
                    { "x", (pageWidth() - m_imageSize.width()) / 2 },
                    { "y", position.y() },
                    { "width", m_imageSize.width() },
                    { "height", m_imageSize.height() },
                    { "source", m_source }
                }
            };
            return QList<ItemInfo>() << info;
        }
    }
    
    return QList<ItemInfo>();
}

int BookImageBlock::lastVisiblePosition(int fromPos, qreal *height, bool *lastPosition)
{
    Q_UNUSED(fromPos);
    
    if (isEnoughtHeight(*height)) {
        *height = m_imageSize.height();
        *lastPosition = true;
        return 10;
    } else {
        *lastPosition = false;
        return 15;
    }
}

int BookImageBlock::inverseLastVisiblePosition(int fromPos, qreal *height, bool *lastPosition, bool *afterLastPosition)
{
    if (fromPos == 15) {
        *height = 0;
        *lastPosition = true;
        *afterLastPosition = false;
        return 0;
    } else if (isEnoughtHeight(*height)) {
        *lastPosition = !qFuzzyCompare(*height, m_imageSize.height());
        *height = m_imageSize.height();
        *afterLastPosition = false;
        return 15;
    } else {
        *lastPosition = false;
        *afterLastPosition = true;
        return 20;
    }
}

void BookImageBlock::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    auto it = imageSizes.find(m_source);
    if (it != imageSizes.end())
        m_size = it.value();
}

bool BookImageBlock::isEnoughtHeight(qreal height) const
{
    return m_imageSize.height() < height || qFuzzyCompare(height, m_imageSize.height());
}

void BookImageBlock::doSetSize(const QSizeF &size)
{
    if (m_size.isValid()) {
        m_imageSize = m_size.scaled(size.width(), size.height(), Qt::KeepAspectRatio);
    } else {
        m_imageSize = QSize(0, 0);
    }
}
