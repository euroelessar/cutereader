#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblockfactory.h"

struct AuthorInfo
{
    QString firstName;
    QString lastName;
    QString middleName;

    QString toString() const
    {
        QStringList result = QStringList()
                << lastName
                << firstName
                << middleName;

        result.removeAll(QString());

        return result.join(QLatin1Char(' '));
    }
};

struct BodyPosition
{
    int block;
    int position;
};

struct BookTextPosition
{
    int body;
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
    QList<AuthorInfo> authors;
    QStringList genres;
    QUrl cover;
    BodyInfo annotation;
    QList<BodyInfo> bodies;

    QStringList authorsList() const
    {
        QStringList result;
        for (const AuthorInfo &author : authors)
            result << author.toString();
        std::sort(result.begin(), result.end());
        return result;
    }
};

Q_DECLARE_METATYPE(BookInfo)
Q_DECLARE_METATYPE(QList<BookInfo>)

#endif // BOOKINFO_H
