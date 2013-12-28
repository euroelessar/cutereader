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

    ModelAction toAction() const
    {
        ModelAction action;
        action.title = title;
        action.entry = source;

        if (type == QStringLiteral("text/html")) {
            action.type = ModelAction::OpenExternally;
        } else if (type.startsWith(QStringLiteral("application/atom+xml"))) {
            action.type = ModelAction::OpenView;

            ModelData data;
            data.type = ModelData::Opds;
            data.source = source;
            action.entry = QVariant::fromValue(data);
        } else if (relation == QStringLiteral("http://opds-spec.org/acquisition/open-access")) {
            action.type = ModelAction::Download;
        }

        return action;
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

    QUrl source() const
    {
        const auto catalog = QStringLiteral("application/atom+xml;profile=opds-catalog");

        for (const OpdsLink &link : links) {
            if (link.type == catalog && link.title.isEmpty())
                return link.source;
        }
        return QUrl();
    }

    bool isBook() const
    {
        const auto acquisition = QStringLiteral("http://opds-spec.org/acquisition");

        for (const OpdsLink &link : links) {
            if (link.relation.startsWith(acquisition))
                return true;
        }
        return false;
    }

    QVariantList actions() const
    {
        QVariantList result;

        for (const OpdsLink &link : links) {
            if (!link.title.isEmpty())
                result << link.toAction().toMap();
        }

        for (const OpdsLink &link : links) {
            if (link.type.isEmpty())
                continue;
            if (link.type.startsWith(QStringLiteral("application/fb2"))) {
                OpdsLink tmp = link;
                tmp.title = QCoreApplication::translate("OpdsBookModel", "Download book (fb2)");
                result << tmp.toAction().toMap();
            }
        }

        return result;
    }

    ModelData data() const
    {
        ModelData data;

        if (isBook()) {
            data.type = ModelData::BookView;
            data.title = title;
            data.text = htmlContent;
            data.cover = cover();
            data.actions = actions();
        } else {
            data.type = ModelData::Opds;
            data.source = source();
        }

        return data;
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
