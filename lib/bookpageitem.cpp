#include "bookpageitem.h"
#include <QQmlContext>
#include <QThread>

BookPageItem::BookPageItem(QQuickItem *parent) :
    QQuickPaintedItem(parent), m_book(NULL), m_block(0), m_blockPosition(0)
{
    qRegisterMetaType<QList<BookBlock::ItemInfo>>();
}

void BookPageItem::paint(QPainter *painter)
{
    if (!m_book) {
        return;
    }
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        if (position.y() > height() || qFuzzyCompare(position.y(), height()))
            break;
        
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(QSizeF(width(), height()));
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
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(QSizeF(width(), height()));
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
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, height());
    for (int i = m_block; i >= 0; --i) {
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(QSizeF(width(), height()));
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
    
    if (m_block == 0 && m_blockPosition == 0)
        return QVariantMap();
    
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
        update();
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
        
        recreateSubItems();
    }
}

void BookPageItem::componentComplete()
{
    QQuickPaintedItem::componentComplete();

    auto component = new QQmlComponent(qmlEngine(this), this);
    component->setData("import QtQuick 2.0\n\nImage {}\n",
                       QUrl::fromUserInput(QStringLiteral("bpi://noop/image.qml")));
    m_components.insert(QStringLiteral("image"), component);
    
    connect(this, &BookPageItem::widthChanged, this, &BookPageItem::recreateSubItems);
    connect(this, &BookPageItem::heightChanged, this, &BookPageItem::recreateSubItems);
}

void BookPageItem::recreateSubItems()
{
    if (!m_book) {
        return;
    }
    
    QList<BookBlock::ItemInfo> items;
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        if (position.y() > height() || qFuzzyCompare(position.y(), height()))
            break;
        
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(QSizeF(width(), height()));
        qreal tmp = height() - position.y();
        items << block->createItems(position, i == m_block ? m_blockPosition : 0, &tmp);
        position.ry() += tmp;
    }
    
    handleSubItems(items);
}

void BookPageItem::handleSubItems(const QList<BookBlock::ItemInfo> &infos)
{
    QList<QObject *> subItems;
    qSwap(m_subItems, subItems);
    qDeleteAll(subItems);
    
    for (const BookBlock::ItemInfo &info : infos) {
        auto it = m_components.find(info.type);
        if (it == m_components.end()) {
            qWarning() << "unknown component type:" << info.type;
            continue;
        }
        
        QQmlComponent *component = it.value();
        QObject *subItem = component->beginCreate(qmlContext(this));
        subItem->setProperty("parent", QVariant::fromValue(this));
        subItem->setParent(this);
        for (auto it = info.properties.begin(); it != info.properties.end(); ++it) {
            subItem->setProperty(it.key().toLatin1(), it.value());
        }
        component->completeCreate();
        
        m_subItems << subItem;
    }
}
