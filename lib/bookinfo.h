#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblock.h"

struct BookInfo
{
    QUrl source;
    QList<BookBlock::Ptr> blocks;
};

Q_DECLARE_METATYPE(BookInfo)

#endif // BOOKINFO_H
