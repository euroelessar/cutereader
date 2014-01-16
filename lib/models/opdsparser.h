#ifndef OPDSPARSER_H
#define OPDSPARSER_H

#include <QIODevice>
#include <QString>
#include <QUrl>
#include <QList>
#include <QVariantMap>
#include <QCoreApplication>
#include "frontmodel.h"

class QXmlStreamReader;
class OpdsEntry;

struct OpdsLink
{
    QUrl source;
    QString relation;
    QString type;
    QString title;

    QVariantMap toMap() const;
    ModelAction toAction(const QVariant &entry) const;
};

struct OpdsEntry
{
    QString title;
    QString textContent;
    QString htmlContent;
    QList<OpdsLink> links;

    QUrl cover() const;
    QUrl source() const;
    bool isBook() const;
    QVariantList actions() const;
    ModelData data() const;
};

struct OpdsInfo
{
    QString title;
    QList<OpdsLink> links;
    QList<OpdsEntry> entries;

    QUrl next() const;
};

class OpdsParser
{
public:
    OpdsParser();

    OpdsInfo parse(const QUrl &baseUrl, QIODevice *device);

private:
    OpdsEntry readEntry(const QUrl &baseUrl, QXmlStreamReader &in);
    OpdsLink readLink(const QUrl &baseUrl, QXmlStreamReader &in);
};

Q_DECLARE_METATYPE(OpdsEntry)

#endif // OPDSPARSER_H
