#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblockfactory.h"

struct AuthorInfo
{
    QString firstName;
    QString lastName;
    QString middleName;
};

struct BodyPosition
{
    int block;
    int position;
};

struct BodyInfo
{
    QString name;
    QList<BookBlockFactory::Ptr> blocks;
    QHash<QString, BodyPosition> references;
};

struct BookInfo
{
    QUrl source;
    QString title;
    AuthorInfo author;
    QStringList genres;
    QUrl cover;
    BodyInfo annotation;
    QList<BodyInfo> bodies;
};

Q_DECLARE_METATYPE(BookInfo)
Q_DECLARE_METATYPE(QList<BookInfo>)

#endif // BOOKINFO_H
