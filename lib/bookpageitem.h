#ifndef BOOKPAGEITEM_H
#define BOOKPAGEITEM_H

#include <QQuickPaintedItem>
#include <QQmlComponent>
#include "bookitem.h"

class BookPageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(BookItem *book READ book WRITE setBook NOTIFY bookChanged)
    Q_PROPERTY(QVariantMap positionValue READ positionValue WRITE setPositionValue NOTIFY positionValueChanged)
    Q_PROPERTY(QVariantMap nextPage READ nextPage NOTIFY nextPageChanged)
    Q_PROPERTY(QVariantMap previousPage READ previousPage NOTIFY previousPageChanged)
    Q_PROPERTY(QQmlComponent *imageDelegate READ imageDelegate WRITE setImageDelegate NOTIFY imageDelegateChanged)
    Q_PROPERTY(QQmlComponent *linkDelegate READ linkDelegate WRITE setLinkDelegate NOTIFY linkDelegateChanged)

public:
    explicit BookPageItem(QQuickItem *parent = 0);
    virtual void paint(QPainter *painter);

    BookItem *book() const;
    int block() const;
    int blockPosition() const;
    QVariantMap positionValue() const;

    QVariantMap nextPage() const;
    QVariantMap previousPage() const;

    QVariantMap recalcNextPage(QList<BookBlock::Ptr> &cache) const;
    QVariantMap recalcPreviousPage(QList<BookBlock::Ptr> &cache) const;
    void recalcPages();

    Q_INVOKABLE QVariant positionForId(const QString &id) const;

    QQmlComponent *imageDelegate() const;
    QQmlComponent *linkDelegate() const;

signals:
    void bookChanged(BookItem *book);
    void positionValueChanged(const QVariantMap &positionValue);
    void nextPageChanged(const QVariantMap &recalcNextPage);
    void previousPageChanged(const QVariantMap &recalcPreviousPage);
    void imageDelegateChanged(QQmlComponent *imageDelegate);
    void linkDelegateChanged(QQmlComponent *linkDelegate);

public slots:
    void setBook(BookItem *book);
    void setPositionValue(const QVariantMap &positionValue);
    void setImageDelegate(QQmlComponent *imageDelegate);
    void setLinkDelegate(QQmlComponent *linkDelegate);

protected:
    virtual void componentComplete();

protected slots:
    void requestUpdate();
    void recreateSubItems();
    void handleSubItems(const QList<BookBlock::ItemInfo> &infos);

private:
    BookItem *m_book;
    int m_body;
    int m_block;
    int m_blockPosition;
    QQmlComponent *m_imageDelegate;
    QQmlComponent *m_linkDelegate;
    QList<QObject*> m_subItems;
    QVariantMap m_nextPage;
    QVariantMap m_previousPage;

    QMutex m_cacheLock;
    QList<BookBlock::Ptr> m_cache;
};

#endif // BOOKPAGEITEM_H
