#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QMetaType>
#include "bookblockfactory.h"

namespace CuteReader {

struct ModelData;

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
    int paragraph;
    int word;
    int character;

    QVariantMap toMap() const;
    static BookTextPosition fromMap(const QVariantMap &arg);

    bool isValid() const;

    operator bool() const;

    bool operator ==(const BookTextPosition &other) const;
    bool operator !=(const BookTextPosition &other) const;
    bool operator <(const BookTextPosition &other) const;
};


struct ContentNode
{
    ContentNode()
    {
        position = { -1, -1 };
    }

    QStringList title;
    BodyPosition position;
    QList<ContentNode> children;
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
    QString number;
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
    ContentNode contents;

    QStringList authorsList() const;
    QStringList sequencesList() const;
    QVariantList actions() const;
    ModelData toData() const;
};

} //namespace CuteReader

Q_DECLARE_METATYPE(CuteReader::BookInfo)
Q_DECLARE_METATYPE(QList<CuteReader::BookInfo>)

#endif // BOOKINFO_H
