#ifndef BOOKPAGEITEM_H
#define BOOKPAGEITEM_H

#include <QQuickPaintedItem>
#include <QQmlComponent>
#include <QSGTexture>
#include "bookitem.h"

namespace CuteReader {

class BookPageIterator;

class BookPageItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(CuteReader::BookItem *book READ book WRITE setBook NOTIFY bookChanged)
    Q_PROPERTY(QVariantMap positionValue READ positionValue WRITE setPositionValue NOTIFY positionValueChanged)
    Q_PROPERTY(QQmlComponent *imageDelegate READ imageDelegate WRITE setImageDelegate NOTIFY imageDelegateChanged)
    Q_PROPERTY(QQmlComponent *linkDelegate READ linkDelegate WRITE setLinkDelegate NOTIFY linkDelegateChanged)

public:
    explicit BookPageItem(QQuickItem *parent = 0);
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    BookItem *book() const;
    QVariantMap positionValue() const;

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
    void classBegin();
    void componentComplete();

protected slots:
    void requestUpdate();
    virtual void recreateSubItems();
    void handleSubItems(const ItemId &id, const QImage &image, const QList<BookBlock::ItemInfo> &infos);

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
    QImage m_cachedImage;
};

} //namespace CuteReader

#endif // BOOKPAGEITEM_H
