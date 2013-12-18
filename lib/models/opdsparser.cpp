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
                    const QXmlStreamAttributes attributes = in.attributes();
                    OpdsLink link;
                    link.source = baseUrl.resolved(attributes.value(QStringLiteral("href")).toString());
                    link.relation = attributes.value(QStringLiteral("rel")).toString();
                    link.title = attributes.value(QStringLiteral("title")).toString();
                    link.type = attributes.value(QStringLiteral("type")).toString();

                    result.links << link;
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
