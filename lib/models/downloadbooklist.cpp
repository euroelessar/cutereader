#include "downloadbooklist.h"

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

int DownloadBookList::jobsCount(QQmlListProperty<DownloadBookJob> *list)
{
    return reinterpret_cast<DownloadBookList*>(list->data)->m_jobs.size();
}

DownloadBookJob *DownloadBookList::jobAt(QQmlListProperty<DownloadBookJob> *list, int index)
{
    return reinterpret_cast<DownloadBookList*>(list->data)->m_jobs.value(index);
}
