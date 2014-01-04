#ifndef BOOKPAGEITERATOR_H
#define BOOKPAGEITERATOR_H

#include "bookblock.h"
#include "bookinfo.h"
#include "bookpageitem.h"

class BookPageIterator
{
public:
    struct Line
    {
        BookBlock::Ptr block;
        int index;
        QPointF position;
    };

    BookPageIterator(const BookPageItem *page);

    BookTextPosition nextPage(QList<BookBlock::Ptr> &cache) const;
    BookTextPosition previousPage(QList<BookBlock::Ptr> &cache) const;

    void setPosition(const BookTextPosition &position);
    BookTextPosition position() const;

    QList<Line> pageLines(QList<BookBlock::Ptr> &cache) const;

    ItemId id() const;

private:
    BookTextPosition m_position;
    const QList<BookBlockFactory::Ptr> m_blocks;
    const QSizeF m_size;
    const BookStyle m_style;
};

#endif // BOOKPAGEITERATOR_H
