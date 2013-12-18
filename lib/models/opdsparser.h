#ifndef OPDSPARSER_H
#define OPDSPARSER_H

#include <QIODevice>
#include <QString>
#include <QUrl>
#include <QList>
#include <QVariantMap>

class QXmlStreamReader;

struct OpdsLink
{
    QUrl source;
    QString relation;
    QString type;
    QString title;

    QVariantMap toMap() const
    {
        return {
            { QStringLiteral("source"), source },
            { QStringLiteral("relation"), relation },
            { QStringLiteral("type"), type },
            { QStringLiteral("title"), title }
        };
    }
};

struct OpdsEntry
{
    QString title;
    QString textContent;
    QString htmlContent;
    QList<OpdsLink> links;

    QUrl cover() const
    {
        for (const OpdsLink &link : links) {
            if (link.type.startsWith(QStringLiteral("image/")))
                return link.source;
        }
        return QUrl();
    }
};

struct OpdsInfo
{
    QString title;
    QList<OpdsEntry> entries;
};

class OpdsParser
{
public:
    OpdsParser();

    OpdsInfo parse(const QUrl &baseUrl, QIODevice *device);

private:
    OpdsEntry readEntry(const QUrl &baseUrl, QXmlStreamReader &in);
};

Q_DECLARE_METATYPE(OpdsEntry)

#endif // OPDSPARSER_H
