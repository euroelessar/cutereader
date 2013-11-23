#include "bookpageitem.h"

BookPageItem::BookPageItem(QQuickItem *parent) :
    QQuickPaintedItem(parent), m_book(NULL), m_block(0), m_blockPosition(0)
{
}

void BookPageItem::paint(QPainter *painter)
{
    if (!m_book) {
        return;
    }
    
    const QList<BookTextBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        if (position.y() > height() || qFuzzyCompare(position.y(), height()))
            break;
        
        const BookTextBlock::Ptr &block = blocks[i];
        block->setWidth(width());
        qreal tmp = height() - position.y();
        block->draw(painter, position, i == m_block ? m_blockPosition : 0, &tmp);
        position.ry() += tmp;
    }
}

BookItem *BookPageItem::book() const
{
    return m_book;
}

int BookPageItem::block() const
{
    return m_block;
}

int BookPageItem::blockPosition() const
{
    return m_blockPosition;
}

QVariantMap BookPageItem::nextPage() const
{
    if (!m_book) {
        return QVariantMap();
    }
    
    QVariantMap result;
    
    const QList<BookTextBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        if (position.y() > height() || qFuzzyCompare(position.y(), height()))
            break;
        
        const BookTextBlock::Ptr &block = blocks[i];
        block->setWidth(width());
        qreal tmp = height() - position.y();
        bool lastPosition = false;
        int blockPosition = block->lastVisiblePosition(i == m_block ? m_blockPosition : 0, &tmp, &lastPosition);
        position.ry() += tmp;
        
        if (!lastPosition) {
            result[QStringLiteral("block")] = i;
            result[QStringLiteral("blockPosition")] = blockPosition;
            return result;
        }
    }
    
    return result;
}

QVariantMap BookPageItem::previousPage() const
{
    if (!m_book) {
        return QVariantMap();
    }
    
    QVariantMap result;
    
    const QList<BookTextBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, height());
    for (int i = m_block; i >= 0; --i) {
        if (position.y() < 0 || qFuzzyIsNull(position.y()))
            break;
        
        const BookTextBlock::Ptr &block = blocks[i];
        block->setWidth(width());
        qreal tmp = position.y();
        bool lastPosition = false;
        bool afterLastPosition = false;
        int blockPosition = block->inverseLastVisiblePosition(i == m_block ? m_blockPosition : std::numeric_limits<int>::max(),
                                                              &tmp, &lastPosition, &afterLastPosition);
        
        if (afterLastPosition) {
            result[QStringLiteral("block")] = i + 1;
            result[QStringLiteral("blockPosition")] = 0;
            return result;
        }
        
        position.ry() -= tmp;
        
        if (!lastPosition) {
            result[QStringLiteral("block")] = i;
            result[QStringLiteral("blockPosition")] = blockPosition;
            return result;
        }
    }
    
    result[QStringLiteral("block")] = 0;
    result[QStringLiteral("blockPosition")] = 0;
    
    return result;
}

QVariantMap BookPageItem::positionValue() const
{
    QVariantMap result;
    result[QStringLiteral("block")] = m_block;
    result[QStringLiteral("blockPosition")] = m_blockPosition;
    return result;
}

void BookPageItem::setBook(BookItem *book)
{
    if (m_book != book) {
        m_book = book;
        emit bookChanged(book);
    }
}

void BookPageItem::setBlock(int block)
{
    if (m_block != block) {
        m_block = block;
        emit blockChanged(block);
        emit positionValueChanged(positionValue());
        update();
    }
}

void BookPageItem::setBlockPosition(int blockPosition)
{
    if (m_blockPosition != blockPosition) {
        m_blockPosition = blockPosition;
        emit blockPositionChanged(blockPosition);
        emit positionValueChanged(positionValue());
        update();
    }
}

void BookPageItem::setPositionValue(const QVariantMap &positionValue)
{
    int block = positionValue[QStringLiteral("block")].toInt();
    int blockPosition = positionValue[QStringLiteral("blockPosition")].toInt();
    
    if (m_block != block || m_blockPosition != blockPosition) {
        m_block = block;
        m_blockPosition = blockPosition;
        emit positionValueChanged(positionValue);
        emit blockChanged(m_block);
        emit blockPositionChanged(m_blockPosition);
        update();
    }
}
