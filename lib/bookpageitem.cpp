#include "bookpageitem.h"
#include <QQmlContext>
#include <QThread>

BookPageItem::BookPageItem(QQuickItem *parent) :
    QQuickPaintedItem(parent), m_book(NULL), m_block(0), m_blockPosition(0)
{
    qRegisterMetaType<QList<BookBlock::ItemInfo>>();
}

static inline bool fuzzyEqual(qreal first, qreal second)
{
    return qFuzzyCompare(first, second);
}

static inline bool fuzzyLess(qreal first, qreal second)
{
    return first < second && !fuzzyEqual(first, second);
}

static inline bool fuzzyLessOrEqual(qreal first, qreal second)
{
    return first < second || fuzzyEqual(first, second);
}

void BookPageItem::paint(QPainter *painter)
{
    if (!m_book)
        return;
    
    const QSizeF pageSize(width(), height());
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(pageSize);
        
        int lineNumber = (m_block == i ? block->lineForPosition(m_blockPosition) : 0);
        const int linesCount = block->linesCount();
        for (int j = lineNumber; j < linesCount; ++j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(height() - position.y(), info.height))
                return;
            block->draw(painter, position, j);
            position.ry() += info.height;
        }
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

QVariantMap createPosition(int block, int blockPosition)
{
    QVariantMap result;
    result[QStringLiteral("block")] = block;
    result[QStringLiteral("blockPosition")] = blockPosition;
    return result;
}

static QVariantMap nextLinePosition(const QList<BookBlock::Ptr> &blocks, const QSizeF &size, int block, int line)
{
    BookBlock::Ptr currentBlock = blocks[block];
    if (line + 1 < currentBlock->linesCount())
        return createPosition(block, currentBlock->lineInfo(line + 1).start);
    
    while (block + 1 < blocks.size()) {
        ++block;
        currentBlock = blocks[block];
        currentBlock->setSize(size);
        
        if (currentBlock->linesCount() == 0)
            continue;
        return createPosition(block, currentBlock->lineInfo(0).start);
    }
    
    return QVariantMap();
}

QVariantMap BookPageItem::nextPage() const
{
    if (!m_book)
        return QVariantMap();
    
    const QSizeF pageSize(width(), height());
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    qreal heightDelta = height();
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(pageSize);
        
        int lineNumber = (m_block == i ? block->lineForPosition(m_blockPosition) : 0);
        const int linesCount = block->linesCount();
        for (int j = lineNumber; j < linesCount; ++j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return createPosition(i, info.start);
            heightDelta -= info.height;
        }
    }
    
    return QVariantMap();
}

QVariantMap BookPageItem::previousPage() const
{
    if (!m_book || (m_block == 0 && m_blockPosition == 0))
        return QVariantMap();
    
    const QSizeF pageSize(width(), height());
    
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    qreal heightDelta = height();
    
    blocks[m_block]->setSize(pageSize);
    int startBlock = m_block;
    int startLine = blocks[m_block]->lineForPosition(m_blockPosition);
    
    if (startLine == 0) {
        --startBlock;
        BookBlock::Ptr block = blocks[startBlock];
        block->setSize(pageSize);
        startLine = block->linesCount();
    }
    
    for (int i = startBlock; i >= 0; --i) {
        BookBlock::Ptr block = blocks[i];
        block->setSize(pageSize);
        
        int lineNumber = (startBlock == i ? startLine : block->linesCount()) - 1;
        while (lineNumber < 0) {
            --i;
            BookBlock::Ptr block = blocks[i];
            block->setSize(pageSize);
            lineNumber = block->linesCount() - 1;
        }
        for (int j = lineNumber; j >= 0; --j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return nextLinePosition(blocks, pageSize, i, j);
            heightDelta -= info.height;
        }
    }
    
    return createPosition(0, 0);
}

QVariantMap BookPageItem::positionValue() const
{
    return createPosition(m_block, m_blockPosition);
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
    if (!m_book)
        return;
    
    const QSizeF pageSize(width(), height());
    QPointF position(0, 0);
    
    QList<BookBlock::ItemInfo> items;
    const QList<BookBlock::Ptr> blocks = m_book->blocks();
    
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i];
        block->setSize(pageSize);
        
        int lineNumber = (m_block == i ? block->lineForPosition(m_blockPosition) : 0);
        const int linesCount = block->linesCount();
        for (int j = lineNumber; j < linesCount; ++j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(height() - position.y(), info.height)) {
                handleSubItems(items);
                return;
            }
            items << block->createItems(position, j);
            position.ry() += info.height;
        }
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
