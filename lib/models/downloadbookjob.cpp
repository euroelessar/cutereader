#include "downloadbookjob.h"

namespace CuteReader {

DownloadBookJob::DownloadBookJob(QObject *parent) :
    QObject(parent), m_progress(0)
{
}

qreal DownloadBookJob::progress() const
{
    return m_progress;
}

QString DownloadBookJob::title() const
{
    return m_title;
}

QUrl DownloadBookJob::cover() const
{
    return m_cover;
}

QUrl DownloadBookJob::source() const
{
    return m_source;
}

} //namespace CuteReader
