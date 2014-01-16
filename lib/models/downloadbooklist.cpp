#include "downloadbooklist.h"
#include "opdsdownloadjob.h"
#include <QQmlEngine>

namespace CuteReader {

DownloadBookList::DownloadBookList(QObject *parent) :
    QObject(parent)
{
}

QQmlListProperty<DownloadBookJob> DownloadBookList::model()
{
    return QQmlListProperty<DownloadBookJob>(this, this, jobsCount, jobAt);
}

void DownloadBookList::addJob(DownloadBookJob *job)
{
    m_jobs << job;
    emit modelChanged(model());
}

QUrl DownloadBookList::baseDir() const
{
    return m_baseDir;
}

void DownloadBookList::setBaseDir(const QUrl &baseDir)
{
    if (m_baseDir != baseDir) {
        m_baseDir = baseDir;
        emit baseDirChanged(baseDir);
    }
}

void DownloadBookList::download(const QVariantMap &info)
{
    const QString type = info.value(QStringLiteral("type")).toString();
    Q_ASSERT(type == QStringLiteral("opds"));

    const QUrl source = info.value(QStringLiteral("source")).toUrl();
    Q_ASSERT(source.isValid());

    Q_ASSERT(info.value(QStringLiteral("entry")).canConvert(qMetaTypeId<OpdsEntry>()));
    const OpdsEntry entry = info.value(QStringLiteral("entry")).value<OpdsEntry>();

    auto manager = qmlEngine(this)->networkAccessManager();
    addJob(new OpdsDownloadJob(this, manager, entry, source));
}

int DownloadBookList::jobsCount(QQmlListProperty<DownloadBookJob> *list)
{
    return reinterpret_cast<DownloadBookList*>(list->data)->m_jobs.size();
}

DownloadBookJob *DownloadBookList::jobAt(QQmlListProperty<DownloadBookJob> *list, int index)
{
    return reinterpret_cast<DownloadBookList*>(list->data)->m_jobs.value(index);
}

} //namespace CuteReader
