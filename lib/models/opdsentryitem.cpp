#include "opdsentryitem.h"
#include <QMimeDatabase>
#include <QDebug>
#include <QQmlEngine>
#include <QNetworkAccessManager>
#include <qqml.h>

OpdsEntryItem::OpdsEntryItem(QObject *parent) :
    QObject(parent)
{
}

QVariant OpdsEntryItem::entry() const
{
    return QVariant::fromValue(m_entry);
}

QUrl OpdsEntryItem::cover() const
{
    return m_entry.cover();
}

QString OpdsEntryItem::title() const
{
    return m_entry.title;
}

QString OpdsEntryItem::content() const
{
    return m_entry.htmlContent;
}

QVariantList OpdsEntryItem::actions() const
{
    QVariantList result;

    for (const OpdsLink &link : m_entry.links) {
        if (!link.title.isEmpty())
            result << link.toMap();
    }

    for (const OpdsLink &link : m_entry.links) {
        if (link.type.isEmpty())
            continue;
        if (link.type.startsWith(QStringLiteral("application/fb2"))) {
            OpdsLink tmp = link;
            tmp.title = tr("Download book");
            result << tmp.toMap();
        }
    }

    return result;
}

void OpdsEntryItem::download(const QUrl &source)
{
    auto manager = qmlEngine(this)->networkAccessManager();

    QNetworkRequest request(source);
    manager->get(request);
}

void OpdsEntryItem::setEntry(const QVariant &entry)
{
    m_entry = entry.value<OpdsEntry>();
    emit entryChanged(entry);
}
