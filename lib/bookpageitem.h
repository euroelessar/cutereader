#ifndef BOOKPAGEITEM_H
#define BOOKPAGEITEM_H

#include <QQuickPaintedItem>
#include <QQmlComponent>
#include "bookitem.h"

class BookPageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(BookItem *book READ book WRITE setBook NOTIFY bookChanged)
    Q_PROPERTY(int block READ block NOTIFY blockChanged)
    Q_PROPERTY(int blockPosition READ blockPosition NOTIFY blockPositionChanged)
    Q_PROPERTY(QVariantMap positionValue READ positionValue WRITE setPositionValue NOTIFY positionValueChanged)
    Q_PROPERTY(QVariantMap nextPage READ nextPage NOTIFY nextPageChanged)
    Q_PROPERTY(QVariantMap previousPage READ previousPage NOTIFY previousPageChanged)

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

signals:
    void bookChanged(BookItem *book);
    void blockChanged(int block);
    void blockPositionChanged(int blockPosition);
    void positionValueChanged(const QVariantMap &positionValue);
    void nextPageChanged(const QVariantMap &recalcNextPage);
    void previousPageChanged(const QVariantMap &recalcPreviousPage);

public slots:
    void setBook(BookItem *book);
    void setPositionValue(const QVariantMap &positionValue);

protected:
    virtual void componentComplete();

protected slots:
    void requestUpdate();
    void recreateSubItems();
    void handleSubItems(const QList<BookBlock::ItemInfo> &infos);

private:
    BookItem *m_book;
    int m_block;
    int m_blockPosition;
    QHash<QString, QQmlComponent*> m_components;
    QList<QObject*> m_subItems;
    QVariantMap m_nextPage;
    QVariantMap m_previousPage;

    QMutex m_cacheLock;
    QList<BookBlock::Ptr> m_cache;
};

#endif // BOOKPAGEITEM_H
