#ifndef DOWNLOADLIST_H
#define DOWNLOADLIST_H

#include <QObject>
#include "downloadbookjob.h"

namespace CuteReader {

class DownloadBookList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<DownloadBookJob> model READ model NOTIFY modelChanged)
    Q_PROPERTY(QUrl baseDir READ baseDir WRITE setBaseDir NOTIFY baseDirChanged)
public:
    explicit DownloadBookList(QObject *parent = 0);

    QQmlListProperty<DownloadBookJob> model();

    void addJob(DownloadBookJob *job);
    QUrl baseDir() const;
    void setBaseDir(const QUrl &baseDir);

    Q_INVOKABLE void download(const QVariantMap &info);

signals:
    void modelChanged(const QQmlListProperty<DownloadBookJob> &model);
    void baseDirChanged(const QUrl &baseDir);

private:
    static int jobsCount(QQmlListProperty<DownloadBookJob> *list);
    static DownloadBookJob *jobAt(QQmlListProperty<DownloadBookJob> *list, int index);

    QList<DownloadBookJob*> m_jobs;
    QUrl m_baseDir;
};

} //namespace CuteReader

#endif // DOWNLOADLIST_H
