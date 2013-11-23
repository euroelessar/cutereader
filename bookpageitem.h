#ifndef BOOKPAGEITEM_H
#define BOOKPAGEITEM_H

#include <QQuickPaintedItem>
#include "bookitem.h"

class BookPageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(BookItem *book READ book WRITE setBook NOTIFY bookChanged)
    Q_PROPERTY(int block READ block WRITE setBlock NOTIFY blockChanged)
    Q_PROPERTY(int blockPosition READ blockPosition WRITE setBlockPosition NOTIFY blockPositionChanged)
    Q_PROPERTY(QVariantMap positionValue READ positionValue WRITE setPositionValue NOTIFY positionValueChanged)

public:
    explicit BookPageItem(QQuickItem *parent = 0);
    virtual void paint(QPainter *painter);

    BookItem *book() const;
    int block() const;
    int blockPosition() const;
    QVariantMap positionValue() const;
    
    Q_INVOKABLE QVariantMap nextPage() const;
    Q_INVOKABLE QVariantMap previousPage() const;
    
signals:
    void bookChanged(BookItem *book);
    void blockChanged(int block);
    void blockPositionChanged(int blockPosition);
    void positionValueChanged(const QVariantMap &positionValue);
    
public slots:
    void setBook(BookItem *book);
    void setBlock(int block);
    void setBlockPosition(int blockPosition);
    
    void setPositionValue(const QVariantMap &positionValue);
    
private:
    BookItem *m_book;
    int m_block;
    int m_blockPosition;
};

#endif // BOOKPAGEITEM_H
