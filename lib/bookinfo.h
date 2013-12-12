#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblockfactory.h"

struct BookInfo
{
    QUrl source;
    QString title;
    QUrl cover;
    QList<BookBlockFactory::Ptr> annotation;
    QList<BookBlockFactory::Ptr> blocks;
};

Q_DECLARE_METATYPE(BookInfo)
Q_DECLARE_METATYPE(QList<BookInfo>)

#endif // BOOKINFO_H
