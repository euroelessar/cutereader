#include "opdsdownloadjob.h"
#include "downloadbooklist.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>
#include <QSaveFile>

OpdsDownloadJob::OpdsDownloadJob(DownloadBookList *parent, QNetworkAccessManager *manager,
                                 const OpdsEntry &entry, const QUrl &source) :
    DownloadBookJob(parent)
{
    m_source = source;
    m_title = entry.title;
    m_cover = entry.cover();

    Q_ASSERT(parent->baseDir().isLocalFile());

    QNetworkRequest request(source);
    QNetworkReply *reply = manager->get(request);
    m_reply = reply;

    QPointer<OpdsDownloadJob> guard(this);
    QSaveFile *file = new QSaveFile(reply);

    connect(m_reply, &QNetworkReply::downloadProgress,
            [this, guard] (qint64 bytesReceived, qint64 bytesTotal) {
        if (!guard)
            return;

        m_progress = bytesReceived * 1.0 / bytesTotal;
        emit progressChanged(m_progress);
    });

    connect(m_reply, &QNetworkReply::finished,
            [this, reply, file, guard] () {
        reply->deleteLater();

        if (reply->error() == QNetworkReply::NoError)
            file->commit();
    });

    connect(m_reply, &QNetworkReply::readyRead,
            [reply, file] {
        file->write(reply->readAll());
    });
}
