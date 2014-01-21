#include "opdsdownloadjob.h"
#include "downloadbooklist.h"
#include <cstdlib>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>
#include <QSaveFile>
#include <QDir>
#include <QDebug>

static QString createBookPath(const QUrl &source)
{
    qDebug() << "source" << source;

    QString path = source.host();
    if (path.isEmpty())
        path = QStringLiteral("unknown.host");
    if (path.startsWith(QStringLiteral("www.")))
        path.remove(0, 4);

    path += QLatin1Char('/');
    path += source.path();

    qDebug() << "path" << path;

    std::_Exit(0);
    return QString();
}

//static QString path = createBookPath(QUrl(QStringLiteral("http://flibusta.net/b/331129/fb2")));

namespace CuteReader {

OpdsDownloadJob::OpdsDownloadJob(DownloadBookList *parent, QNetworkAccessManager *manager,
                                 const OpdsEntry &entry, const QUrl &source) :
    DownloadBookJob(parent)
{
    m_source = source;
    m_title = entry.title;
    m_cover = entry.cover();
    m_file = new QSaveFile(this);

    Q_ASSERT(parent->baseDir().isLocalFile());
    m_baseDir = parent->baseDir().toLocalFile();

    const QString filePath = m_baseDir.filePath(createBookPath(source));
    const QDir fileDir = QFileInfo(filePath).path();
    if (!fileDir.exists())
        fileDir.mkpath(fileDir.absolutePath());

    m_file->setFileName(filePath);
    m_file->open(QFile::WriteOnly);

    QNetworkRequest request(source);
    setReply(manager->get(request));
}

void OpdsDownloadJob::setReply(QNetworkReply *reply)
{
    m_reply = reply;

    if (m_file->pos() != 0) {
        m_file->seek(0);
        m_file->resize(0);
    }

    QPointer<OpdsDownloadJob> guard(this);

    connect(m_reply, &QNetworkReply::downloadProgress,
            [this, guard] (qint64 bytesReceived, qint64 bytesTotal) {
        if (!guard || bytesTotal == 0)
            return;

        m_progress = bytesReceived * 1.0 / bytesTotal;
        emit progressChanged(m_progress);
    });

    connect(m_reply, &QNetworkReply::finished,
            [this, reply, guard] () {
        reply->deleteLater();

        if (!guard)
            return;

        const QUrl location = reply->header(QNetworkRequest::LocationHeader).toUrl();
        if (location.isValid()) {
            reply->disconnect();
            QNetworkRequest request(location);
            setReply(reply->manager()->get(request));
            return;
        }

        if (reply->error() == QNetworkReply::NoError)
            m_file->commit();
    });

    connect(m_reply, &QNetworkReply::readyRead,
            [this, reply, guard] {
        if (guard)
            m_file->write(reply->readAll());
    });
}

} //namespace CuteReader
