#ifndef BOOKPAGEITEM_H
#define BOOKPAGEITEM_H

#include <QQuickPaintedItem>
#include <QQmlComponent>
#include "bookitem.h"

class BookPageIterator;

class BookPageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(BookItem *book READ book WRITE setBook NOTIFY bookChanged)
    Q_PROPERTY(QVariantMap positionValue READ positionValue WRITE setPositionValue NOTIFY positionValueChanged)
    Q_PROPERTY(QQmlComponent *imageDelegate READ imageDelegate WRITE setImageDelegate NOTIFY imageDelegateChanged)
    Q_PROPERTY(QQmlComponent *linkDelegate READ linkDelegate WRITE setLinkDelegate NOTIFY linkDelegateChanged)

public:
    explicit BookPageItem(QQuickItem *parent = 0);
    virtual void paint(QPainter *painter);

    BookItem *book() const;
    QVariantMap positionValue() const;

    Q_INVOKABLE QVariant positionForId(const QString &id) const;
    Q_INVOKABLE QVariantMap nextPageForPosition(const QVariantMap &position);
    Q_INVOKABLE QVariantMap previousPageForPosition(const QVariantMap &position);

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
    virtual void recreateSubItems();
    void handleSubItems(const QList<BookBlock::ItemInfo> &infos);

private:
    friend class BookPageIterator;

    BookItem *m_book;
    BookTextPosition m_positionValue;
    QQmlComponent *m_imageDelegate;
    QQmlComponent *m_linkDelegate;
    QList<QObject*> m_subItems;
    QVariantMap m_nextPage;
    QVariantMap m_previousPage;

    QMutex m_cacheLock;
    QList<BookBlock::Ptr> m_cache;
};

#endif // BOOKPAGEITEM_H
