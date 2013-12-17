#include "bookpageitem.h"
#include <QQmlContext>
#include <QThread>

BookPageItem::BookPageItem(QQuickItem *parent) :
    QQuickPaintedItem(parent), m_book(NULL),
    m_body(0), m_block(0), m_blockPosition(0),
    m_imageDelegate(NULL), m_linkDelegate(NULL)
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
    const BookStyle style = m_book->style();

    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks(m_body);

    QPointF position(0, 0);
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i]->item(pageSize, style);

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

QVariantMap createPosition(int body, int block, int blockPosition)
{
    QVariantMap result;
    result[QStringLiteral("body")] = body;
    result[QStringLiteral("block")] = block;
    result[QStringLiteral("blockPosition")] = blockPosition;
    return result;
}

static BookBlock::Ptr fetchBlock(int index, const QList<BookBlockFactory::Ptr> &factories, const QSizeF &size, const BookStyle &style, QList<BookBlock::Ptr> &cache)
{
    BookBlock::Ptr block = factories[index]->item(size, style);
    cache << block;
    return block;
}

static QVariantMap nextLinePosition(const QList<BookBlockFactory::Ptr> &factories,
                                    QList<BookBlock::Ptr> &cache,
                                    const QSizeF &size, const BookStyle &style, int body, int block, int line)
{
    BookBlock::Ptr currentBlock = fetchBlock(block, factories, size, style, cache);
    if (line + 1 < currentBlock->linesCount())
        return createPosition(body, block, currentBlock->lineInfo(line + 1).start);

    while (block + 1 < factories.size()) {
        ++block;
        currentBlock = fetchBlock(block, factories, size, style, cache);

        if (currentBlock->linesCount() == 0)
            continue;
        return createPosition(body, block, currentBlock->lineInfo(0).start);
    }

    return QVariantMap();
}

QVariantMap BookPageItem::recalcNextPage(QList<BookBlock::Ptr> &cache) const
{
    if (!m_book)
        return QVariantMap();

    const QSizeF pageSize(width(), height());
    const BookStyle style = m_book->style();

    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks(m_body);
    qreal heightDelta = height();
    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = fetchBlock(i, blocks, pageSize, style, cache);

        int lineNumber = (m_block == i ? block->lineForPosition(m_blockPosition) : 0);
        const int linesCount = block->linesCount();
        for (int j = lineNumber; j < linesCount; ++j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return createPosition(m_body, i, info.start);
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
    const BookStyle style = m_book->style();

    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks(m_body);
    qreal heightDelta = height();

    if (m_block >= blocks.size() || m_block < 0)
        return QVariantMap();

    int startBlock = m_block;
    int startLine = fetchBlock(m_block, blocks, pageSize, style, cache)->lineForPosition(m_blockPosition);

    if (startLine == 0) {
        --startBlock;
        BookBlock::Ptr block = fetchBlock(startBlock, blocks, pageSize, style, cache);
        startLine = block->linesCount();
    }

    for (int i = startBlock; i >= 0; --i) {
        BookBlock::Ptr block = fetchBlock(i, blocks, pageSize, style, cache);

        int lineNumber = (startBlock == i ? startLine : block->linesCount()) - 1;
        while (lineNumber < 0) {
            --i;
            BookBlock::Ptr block = fetchBlock(i, blocks, pageSize, style, cache);
            lineNumber = block->linesCount() - 1;
        }
        for (int j = lineNumber; j >= 0; --j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return nextLinePosition(blocks, cache, pageSize, style, m_body, i, j);
            heightDelta -= info.height;
        }
    }

    return createPosition(m_body, 0, 0);
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

QVariant BookPageItem::positionForId(const QString &id) const
{
    BookTextPosition position = m_book->positionForId(id);
    if (position.body < 0)
        return QVariant();

    return createPosition(position.body, position.block, position.position);
}

QQmlComponent *BookPageItem::imageDelegate() const
{
    return m_imageDelegate;
}

QQmlComponent *BookPageItem::linkDelegate() const
{
    return m_linkDelegate;
}

QVariantMap BookPageItem::positionValue() const
{
    return createPosition(m_body, m_block, m_blockPosition);
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
        connect(m_book, &BookItem::styleChanged, this, &BookPageItem::requestUpdate);

        emit bookChanged(book);
        requestUpdate();
    }
}

void BookPageItem::setPositionValue(const QVariantMap &positionValue)
{
    int body = positionValue.value(QStringLiteral("body")).toInt();
    int block = positionValue.value(QStringLiteral("block")).toInt();
    int blockPosition = positionValue.value(QStringLiteral("blockPosition")).toInt();

    if (m_body != body || m_block != block || m_blockPosition != blockPosition) {
        m_body = body;
        m_block = block;
        m_blockPosition = blockPosition;
        emit positionValueChanged(positionValue);
        requestUpdate();
    }
}

void BookPageItem::setImageDelegate(QQmlComponent *imageDelegate)
{
    if (m_imageDelegate != imageDelegate) {
        m_imageDelegate = imageDelegate;
        emit imageDelegateChanged(imageDelegate);
    }
}

void BookPageItem::setLinkDelegate(QQmlComponent *linkDelegate)
{
    if (m_linkDelegate != linkDelegate) {
        m_linkDelegate = linkDelegate;
        emit linkDelegateChanged(linkDelegate);
    }
}

void BookPageItem::componentComplete()
{
    QQuickPaintedItem::componentComplete();

    if (!m_imageDelegate) {
        m_imageDelegate = new QQmlComponent(qmlEngine(this), this);
        m_imageDelegate->setData("import QtQuick 2.0\n"
                                 "\n"
                                 "Image {\n"
                                 "}\n",
                                 QUrl::fromUserInput(QStringLiteral("bpi://noop/Image.qml")));
    }
    if (!m_linkDelegate) {
        m_linkDelegate = new QQmlComponent(qmlEngine(this), this);
        m_linkDelegate->setData("import QtQuick 2.0\n"
                                "\n"
                                "Rectangle {\n"
                                "   color: '#ff0000'\n"
                                "   property variant positionValue\n"
                                "}\n",
                                QUrl::fromUserInput(QStringLiteral("bpi://noop/Link.qml")));
    }

    connect(this, &BookPageItem::widthChanged, this, &BookPageItem::requestUpdate);
    connect(this, &BookPageItem::heightChanged, this, &BookPageItem::requestUpdate);
}

void BookPageItem::requestUpdate()
{
    recalcPages();
    recreateSubItems();
    update();
}

void BookPageItem::recreateSubItems()
{
    if (!m_book)
        return;

    const QSizeF pageSize(width(), height());
    const BookStyle style = m_book->style();
    QPointF position(0, 0);

    QList<BookBlock::ItemInfo> items;
    const QList<BookBlockFactory::Ptr> blocks = m_book->blocks(m_body);

    for (int i = m_block; i < blocks.size(); ++i) {
        const BookBlock::Ptr &block = blocks[i]->item(pageSize, style);

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
    for (auto object : subItems)
        object->deleteLater();

    for (const BookBlock::ItemInfo &info : infos) {
        QQmlComponent *component = NULL;

        if (info.type == QStringLiteral("image"))
            component = m_imageDelegate;
        else if (info.type == QStringLiteral("link"))
            component = m_linkDelegate;
        else
            qWarning() << "unknown component type:" << info.type;

        if (!component)
            continue;

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
