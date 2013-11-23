#ifndef BOOKSURFACEITEM_H
#define BOOKSURFACEITEM_H

#include <QQuickPaintedItem>
#include <hyphen.h>
#include "booktextblock.h"
#include "bookblockitem.h"

class BookSurfaceItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit BookSurfaceItem(QQuickItem *parent = 0);
    ~BookSurfaceItem();

    virtual void paint(QPainter *painter);
    
    QList<BookTextBlock::Ptr> blocks() const;
    
signals:
    
public slots:
    
private:
    QList<BookTextBlock::Ptr> m_blocks;
    HyphenDict *m_dict;
};

#endif // BOOKSURFACEITEM_H
