#ifndef BOOKSTYLE_H
#define BOOKSTYLE_H

#include <QObject>
#include <QReadWriteLock>
#include "booktextstyleitem.h"
#include "bookblock.h"

class BookStyleItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BookTextStyleItem *base READ base CONSTANT FINAL)
public:
    explicit BookStyleItem(QObject *parent = 0);

    BookTextStyleItem *base() const { return m_formats[Format::Base]; }

    BookStyle style() const;

signals:
    void changed(const BookStyle &style);

private:
    QVector<BookTextStyleItem *> m_formats;
    BookStyle m_style;
    mutable QReadWriteLock m_lock;
};

#endif // BOOKSTYLE_H
