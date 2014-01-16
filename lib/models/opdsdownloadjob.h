#ifndef OPDSDOWNLOADJOB_H
#define OPDSDOWNLOADJOB_H

#include "downloadbookjob.h"
#include "opdsparser.h"
#include <QNetworkAccessManager>
#include <QDir>

namespace CuteReader {

class DownloadBookList;
class QSaveFile;

class OpdsDownloadJob : public DownloadBookJob
{
    Q_OBJECT
public:
    explicit OpdsDownloadJob(DownloadBookList *parent, QNetworkAccessManager *manager, const OpdsEntry &entry, const QUrl &source);

    void setReply(QNetworkReply *reply);

private:
    QNetworkReply *m_reply;
    QDir m_baseDir;
    QSaveFile *m_file;
};

} //namespace CuteReader

#endif // OPDSDOWNLOADJOB_H
