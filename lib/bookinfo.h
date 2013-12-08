#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblock.h"

struct BookInfo
{
    QUrl source;
    QString title;
    QUrl cover;
    QList<BookBlock::Ptr> annotation;
    QList<BookBlock::Ptr> blocks;
};

Q_DECLARE_METATYPE(BookInfo)
Q_DECLARE_METATYPE(QList<BookInfo>)

#endif // BOOKINFO_H
