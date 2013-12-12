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

    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks();
    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i]->item(pageSize);

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

static BookBlock::Ptr fetchBlock(int index, const QList<BookBlockFactory::Ptr> &factories, const QSizeF &size, QList<BookBlock::Ptr> &cache)
{
    BookBlock::Ptr block = factories[index]->item(size);
    cache << block;
    return block;
}

static QVariantMap nextLinePosition(const QList<BookBlockFactory::Ptr> &factories,
                                    QList<BookBlock::Ptr> &cache,
                                    const QSizeF &size, int block, int line)
{
    BookBlock::Ptr currentBlock = fetchBlock(block, factories, size, cache);
    if (line + 1 < currentBlock->linesCount())
        return createPosition(block, currentBlock->lineInfo(line + 1).start);

    while (block + 1 < factories.size()) {
        ++block;
        currentBlock = fetchBlock(block, factories, size, cache);

        if (currentBlock->linesCount() == 0)
            continue;
        return createPosition(block, currentBlock->lineInfo(0).start);
    }

    return QVariantMap();
}

QVariantMap BookPageItem::recalcNextPage(QList<BookBlock::Ptr> &cache) const
{
    if (!m_book)
        return QVariantMap();

    const QSizeF pageSize(width(), height());

    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks();
    qreal heightDelta = height();
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = fetchBlock(i, blocks, pageSize, cache);

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

QVariantMap BookPageItem::recalcPreviousPage(QList<BookBlock::Ptr> &cache) const
{
    if (!m_book || (m_block == 0 && m_blockPosition == 0))
        return QVariantMap();

    const QSizeF pageSize(width(), height());

    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks();
    qreal heightDelta = height();

    int startBlock = m_block;
    int startLine = fetchBlock(m_block, blocks, pageSize, cache)->lineForPosition(m_blockPosition);

    if (startLine == 0) {
        --startBlock;
        BookBlock::Ptr block = fetchBlock(startBlock, blocks, pageSize, cache);
        startLine = block->linesCount();
    }

    for (int i = startBlock; i >= 0; --i) {
        BookBlock::Ptr block = fetchBlock(i, blocks, pageSize, cache);

        int lineNumber = (startBlock == i ? startLine : block->linesCount()) - 1;
        while (lineNumber < 0) {
            --i;
            BookBlock::Ptr block = fetchBlock(i, blocks, pageSize, cache);
            lineNumber = block->linesCount() - 1;
        }
        for (int j = lineNumber; j >= 0; --j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return nextLinePosition(blocks, cache, pageSize, i, j);
            heightDelta -= info.height;
        }
    }

    return createPosition(0, 0);
}

void BookPageItem::recalcPages()
{
    QList<BookBlock::Ptr> cache;

    QVariantMap nextPage = recalcNextPage(cache);
    if (nextPage != m_nextPage) {
        m_nextPage = nextPage;
        emit nextPageChanged(m_nextPage);
    }

    QVariantMap previousPage = recalcPreviousPage(cache);
    if (previousPage != m_previousPage) {
        m_previousPage = previousPage;
        emit previousPageChanged(m_previousPage);
    }

    QMutexLocker locker(&m_cacheLock);
    m_cache << cache;
    if (m_cache.size() > 3)
        m_cache.removeFirst();
}

QVariantMap BookPageItem::positionValue() const
{
    return createPosition(m_block, m_blockPosition);
}

QVariantMap BookPageItem::nextPage() const
{
    return m_nextPage;
}

QVariantMap BookPageItem::previousPage() const
{
    return m_previousPage;
}

void BookPageItem::setBook(BookItem *book)
{
    if (m_book != book) {
        if (m_book)
            disconnect(m_book, 0, this, 0);

        m_book = book;
        connect(m_book, &BookItem::stateChanged, this, &BookPageItem::requestUpdate);

        emit bookChanged(book);
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
        requestUpdate();

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

void BookPageItem::requestUpdate()
{
    recalcPages();
    update();
}

void BookPageItem::recreateSubItems()
{
    if (!m_book)
        return;

    const QSizeF pageSize(width(), height());
    QPointF position(0, 0);

    QList<BookBlock::ItemInfo> items;
    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks();

    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i]->item(pageSize);

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
