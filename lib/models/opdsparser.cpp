#include "opdsparser.h"
#include <QXmlStreamReader>
#include <QDebug>

OpdsParser::OpdsParser()
{
}

OpdsInfo OpdsParser::parse(const QUrl &baseUrl, QIODevice *device)
{
    QXmlStreamReader in(device);
    OpdsInfo result;

    int depth = 0;

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (depth == 2) {
                if (in.name() == QStringLiteral("title")) {
                    result.title = in.readElementText(QXmlStreamReader::IncludeChildElements);
                    --depth;
                } else if (in.name() == QStringLiteral("entry")) {
                    result.entries << readEntry(baseUrl, in);
                    --depth;
                } else if (in.name() == QStringLiteral("link")) {
                    result.links << readLink(baseUrl, in);
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            --depth;
            break;
        case QXmlStreamReader::Characters:
            break;
        default:
            break;
        }
    }

    return result;
}

OpdsEntry OpdsParser::readEntry(const QUrl &baseUrl, QXmlStreamReader &in)
{
    int depth = 1;
    OpdsEntry result;

    while (in.readNext() != QXmlStreamReader::Invalid) {
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            ++depth;

            if (depth == 2) {
                if (in.name() == QStringLiteral("title")) {
                    result.title = in.readElementText(QXmlStreamReader::IncludeChildElements);
                    --depth;
                } else if (in.name() == QStringLiteral("content")
                           && in.attributes().value(QStringLiteral("type")) == QStringLiteral("text")) {
                    result.textContent = in.readElementText(QXmlStreamReader::IncludeChildElements);
                    --depth;
                } else if (in.name() == QStringLiteral("content")
                           && in.attributes().value(QStringLiteral("type")) == QStringLiteral("text/html")) {
                    result.htmlContent = in.readElementText(QXmlStreamReader::IncludeChildElements);
                    --depth;
                } else if (in.name() == QStringLiteral("link")) {
                    result.links << readLink(baseUrl, in);
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if (depth == 1) {
                return result;
            }

            --depth;
            break;
        case QXmlStreamReader::Characters:
            break;
        default:
            break;
        }
    }

    return result;
}

OpdsLink OpdsParser::readLink(const QUrl &baseUrl, QXmlStreamReader &in)
{
    Q_ASSERT(in.name() == QStringLiteral("link"));

    const QXmlStreamAttributes attributes = in.attributes();
    OpdsLink link;
    link.source = baseUrl.resolved(attributes.value(QStringLiteral("href")).toString());
    link.relation = attributes.value(QStringLiteral("rel")).toString();
    link.title = attributes.value(QStringLiteral("title")).toString();
    link.type = attributes.value(QStringLiteral("type")).toString();
    return link;
}


QVariantMap OpdsLink::toMap() const
{
    return {
        { QStringLiteral("source"), source },
        { QStringLiteral("relation"), relation },
        { QStringLiteral("type"), type },
        { QStringLiteral("title"), title }
    };
}

ModelAction OpdsLink::toAction(const QVariant &entry) const
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
        action.entry = QVariantMap {
        { QStringLiteral("type"), QStringLiteral("opds") },
        { QStringLiteral("source"), source },
        { QStringLiteral("entry"), entry }
    };
    }

    return action;
}


QUrl OpdsEntry::cover() const
{
    for (const OpdsLink &link : links) {
        if (link.type.startsWith(QStringLiteral("image/")))
            return link.source;
    }
    return QUrl();
}

QUrl OpdsEntry::source() const
{
    const auto catalog = QStringLiteral("application/atom+xml;profile=opds-catalog");

    for (const OpdsLink &link : links) {
        if (link.type == catalog && link.title.isEmpty())
            return link.source;
    }
    return QUrl();
}

bool OpdsEntry::isBook() const
{
    const auto acquisition = QStringLiteral("http://opds-spec.org/acquisition");

    for (const OpdsLink &link : links) {
        if (link.relation.startsWith(acquisition))
            return true;
    }
    return false;
}

QVariantList OpdsEntry::actions() const
{
    QVariantList result;
    QVariant entry = QVariant::fromValue(*this);

    for (const OpdsLink &link : links) {
        if (!link.title.isEmpty())
            result << link.toAction(entry).toMap();
    }

    for (const OpdsLink &link : links) {
        if (link.type.isEmpty())
            continue;
        if (link.type.startsWith(QStringLiteral("application/fb2"))) {
            OpdsLink tmp = link;
            tmp.title = QCoreApplication::translate("OpdsBookModel", "Download book (fb2)");
            result << tmp.toAction(entry).toMap();
        }
    }

    return result;
}

ModelData OpdsEntry::data() const
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


QUrl OpdsInfo::next() const
{
    const auto catalog = QStringLiteral("application/atom+xml;profile=opds-catalog");

    for (const OpdsLink &link : links) {
        if (link.type == catalog && link.relation == QStringLiteral("next"))
            return link.source;
    }
    return QUrl();
}
