#include "bookimageblock.h"
#include <QQuickView>
#include <QDebug>

BookImageBlock::BookImageBlock(const BookImageBlockData::Ptr &data, const QSizeF &size, const QWeakPointer<BookBlockFactory> &factory)
    : BookBlock(size, factory), m_data(data)
{
    if (data->size.isValid()) {
        m_imageSize = m_data->size.scaled(qMin<int>(m_data->size.width(), size.width()),
                                    qMin<int>(m_data->size.height(), size.height()),
                                    Qt::KeepAspectRatio);
    } else {
        m_imageSize = QSize(0, 0);
    }
}

void BookImageBlock::draw(QPainter *painter, const QPointF &position, int line) const
{
    Q_UNUSED(painter);
    Q_UNUSED(position);
    Q_UNUSED(line);
}

QList<BookBlock::ItemInfo> BookImageBlock::createItems(const QPointF &position, int line) const
{
    Q_UNUSED(line);

    if (m_data->size.isValid()) {
        return QList<ItemInfo>() << ItemInfo {
            QStringLiteral("image"),
            {
                { "x", (pageWidth() - m_imageSize.width()) / 2 },
                { "y", position.y() },
                { "width", m_imageSize.width() },
                { "height", m_imageSize.height() },
                { "source", m_data->source }
            }
        };
    }

    return QList<ItemInfo>();
}

int BookImageBlock::linesCount() const
{
    if (m_imageSize.isValid())
        return 1;
    return 0;
}

int BookImageBlock::lineForPosition(int position)
{
    Q_UNUSED(position);
    return 0;
}

BookBlock::LineInfo BookImageBlock::lineInfo(int line)
{
    Q_UNUSED(line);
    return {
        qreal(m_imageSize.isValid() ? m_imageSize.height() : 0),
        0,
        1
    };
}

void BookImageBlock::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    auto it = imageSizes.find(m_data->source);
    if (it != imageSizes.end())
        m_data->size = it.value();
}

void BookImageBlock::doSetSize(const QSizeF &size)
{
    if (m_data->size.isValid()) {
        m_imageSize = m_data->size.scaled(qMin<int>(m_data->size.width(), size.width()),
                                    qMin<int>(m_data->size.height(), size.height()),
                                    Qt::KeepAspectRatio);
    } else {
        m_imageSize = QSize(0, 0);
    }
}
