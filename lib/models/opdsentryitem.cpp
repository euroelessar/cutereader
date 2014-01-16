#include "opdsentryitem.h"
#include "opdsdownloadjob.h"
#include <QMimeDatabase>
#include <QDebug>
#include <QQmlEngine>
#include <qqml.h>

namespace CuteReader {

OpdsEntryItem::OpdsEntryItem(QObject *parent) :
    QObject(parent), m_downloadList(NULL)
{
}

QVariant OpdsEntryItem::entry() const
{
    return QVariant::fromValue(m_entry);
}

QUrl OpdsEntryItem::cover() const
{
    return m_entry.cover;
}

QString OpdsEntryItem::title() const
{
    return m_entry.title;
}

QString OpdsEntryItem::content() const
{
    return m_entry.text;
}

QVariantList OpdsEntryItem::actions() const
{
    return m_entry.actions;
}

DownloadBookList *OpdsEntryItem::downloadList() const
{
    return m_downloadList;
}

void OpdsEntryItem::setEntry(const QVariant &entry)
{
    m_entry = entry.value<ModelData>();
    emit entryChanged(entry);
}

void OpdsEntryItem::setDownloadList(DownloadBookList *downloadList)
{
    if (m_downloadList != downloadList) {
        m_downloadList = downloadList;
        emit downloadListChanged(downloadList);
    }
}

} //namespace CuteReader
