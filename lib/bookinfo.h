#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblockfactory.h"

class ModelData;

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

    QVariantMap toMap() const;
    static BookTextPosition fromMap(const QVariantMap &arg);

    bool isValid() const;

    operator bool() const;

    bool operator ==(const BookTextPosition &other) const;
    bool operator !=(const BookTextPosition &other) const;
    bool operator <(const BookTextPosition &other) const;
};

struct BodyInfo
{
    QString name;
    QList<BookBlockFactory::Ptr> blocks;
    QHash<QString, BodyPosition> references;
};

struct SequenceInfo
{
    QString name;
    int number;
};

struct BookInfo
{
    QUrl source;
    QString title;
    QList<AuthorInfo> authors;
    QStringList genres;
    QList<SequenceInfo> sequences;
    QUrl cover;
    BodyInfo annotation;
    QList<BodyInfo> bodies;

    QStringList authorsList() const;
    QStringList sequencesList() const;
    QVariantList actions() const;
    ModelData toData() const;
};

Q_DECLARE_METATYPE(BookInfo)
Q_DECLARE_METATYPE(QList<BookInfo>)

#endif // BOOKINFO_H
