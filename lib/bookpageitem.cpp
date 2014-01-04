#include "bookpageitem.h"
#include "bookpageiterator.h"
#include "saferunnable.h"
#include <QQmlContext>
#include <QElapsedTimer>
#include <QThread>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

BookPageItem::BookPageItem(QQuickItem *parent) :
    QQuickItem(parent),
    m_book(NULL),
    m_positionValue({ 0, 0, 0 }),
    m_imageDelegate(NULL),
    m_linkDelegate(NULL),
    m_texture(NULL)
{
    qRegisterMetaType<QList<BookBlock::ItemInfo>>();
    setFlag(ItemHasContents);
}

QSGNode *BookPageItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);
    if (!node) {
        node = new QSGSimpleTextureNode;
    }

    if (m_texture) {
        node->setTexture(NULL);
        delete m_texture;
        m_texture = NULL;
    }

    QImage image;
    {
        QMutexLocker locker(&m_cacheLock);
        if (m_cachedImage.isNull()) {
            delete node;
            return NULL;
        }
        image = m_cachedImage;
    }

    m_texture = window()->createTextureFromImage(image);
    node->setTexture(m_texture);
    node->setRect(0, 0, image.width(), image.height());

    return node;
}

BookItem *BookPageItem::book() const
{
    return m_book;
}

QVariant BookPageItem::positionForId(const QString &id) const
{
    return m_book->positionForId(id).toMap();
}

QVariantMap BookPageItem::nextPageForPosition(const QVariantMap &position)
{
    if (!m_book)
        return QVariantMap();

    static quint64 nsecs = 0;
    static quint64 calls = 0;

    QElapsedTimer timer;
    timer.start();

    BookPageIterator it(this);
    it.setPosition(BookTextPosition::fromMap(position));

    QList<BookBlock::Ptr> cache;
    QVariantMap result = it.nextPage(cache).toMap();

    nsecs += timer.nsecsElapsed();
    ++calls;
    qDebug() << "nextPage total:" << nsecs / 1000000. << "ms, average: " << nsecs / 1000000. / calls << "ms";

    return result;
}

QVariantMap BookPageItem::previousPageForPosition(const QVariantMap &position)
{
    if (!m_book)
        return QVariantMap();

    BookPageIterator it(this);
    it.setPosition(BookTextPosition::fromMap(position));

    QList<BookBlock::Ptr> cache;
    return it.previousPage(cache).toMap();
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
    return m_positionValue.toMap();
}

void BookPageItem::setBook(BookItem *book)
{
    if (m_book != book) {
        if (m_book)
            disconnect(m_book, nullptr, this, nullptr);

        m_book = book;
        connect(m_book, &BookItem::stateChanged, this, &BookPageItem::requestUpdate);
        connect(m_book, &BookItem::styleChanged, this, &BookPageItem::requestUpdate);

        emit bookChanged(book);
        requestUpdate();
    }
}

void BookPageItem::setPositionValue(const QVariantMap &arg)
{
    BookTextPosition positionValue = BookTextPosition::fromMap(arg);

    if (positionValue != m_positionValue) {
        m_positionValue = positionValue;
        emit positionValueChanged(positionValue.toMap());
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

void BookPageItem::classBegin()
{
    QQuickItem::classBegin();
}

void BookPageItem::componentComplete()
{
    QQuickItem::componentComplete();

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
    requestUpdate();
}

void BookPageItem::requestUpdate()
{
    // Item is not constructed yet
    if (!m_imageDelegate || !m_linkDelegate)
        return;

    {
        QMutexLocker locker(&m_cacheLock);
        m_cache.clear();
    }

    recreateSubItems();
}

void BookPageItem::recreateSubItems()
{
    if (!m_book)
        return;

    for (auto object : m_subItems)
        object->setProperty("visible", false);

    const BookPageIterator it(this);
    const ItemId id = it.id();

    SafeRunnable::start(this, [this, it, id] () -> SafeRunnable::Handler {
        QList<BookBlock::Ptr> cache;
        QList<BookBlock::ItemInfo> items;

        QSize size = it.id().size().toSize();

        if (size.isEmpty())
            return [] () {};

        size.rwidth() *= 1.1;
        QImage image(size, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);

        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

        for (const auto &line : it.pageLines(cache)) {
            painter.setRenderHint(QPainter::TextAntialiasing, !painter.testRenderHint(QPainter::TextAntialiasing));
            items << line.block->createItems(line.position, line.index);
            line.block->draw(&painter, line.position, line.index);
        }

        return [this, image, items, cache, id] () {
            handleSubItems(id, image, cache, items);
        };
    });
}

void BookPageItem::handleSubItems(const ItemId &id, const QImage &image, const QList<BookBlock::Ptr> &cache, const QList<BookBlock::ItemInfo> &infos)
{
    const BookPageIterator it(this);
    if (it.id() != id)
        return;

    {
        QMutexLocker locker(&m_cacheLock);
        m_cache = cache;
        m_cachedImage = image;
    }

    update();

    QList<QObject *> subItems;
    qSwap(m_subItems, subItems);
    for (auto object : subItems) {
        object->setProperty("visible", false);
        object->deleteLater();
    }

    if (!qmlContext(this) || !qmlContext(this)->parentContext())
        return;

    for (const BookBlock::ItemInfo &info : infos) {
        QQmlComponent *component = NULL;

        if (info.type == QStringLiteral("image"))
            component = m_imageDelegate;
        else if (info.type == QStringLiteral("link"))
            component = m_linkDelegate;
        else
            qWarning() << "unknown component type:" << info.type;

        if (!component || !component->isReady())
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
