#ifndef OPDSDOWNLOADJOB_H
#define OPDSDOWNLOADJOB_H

#include "downloadbookjob.h"
#include "opdsparser.h"
#include <QNetworkAccessManager>

class DownloadBookList;

class OpdsDownloadJob : public DownloadBookJob
{
    Q_OBJECT
public:
    explicit OpdsDownloadJob(DownloadBookList *parent, QNetworkAccessManager *manager, const OpdsEntry &entry, const QUrl &source);

private:
    QNetworkReply *m_reply;
};

#endif // OPDSDOWNLOADJOB_H
