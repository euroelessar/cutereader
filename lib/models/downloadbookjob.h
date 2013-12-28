#ifndef DOWNLOADBOOKJOB_H
#define DOWNLOADBOOKJOB_H

#include <QObject>
#include <qqml.h>

class DownloadBookJob : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString title READ title CONSTANT FINAL)
    Q_PROPERTY(QUrl cover READ cover CONSTANT FINAL)
    Q_PROPERTY(QUrl source READ source CONSTANT FINAL)
public:
    explicit DownloadBookJob(QObject *parent = 0);

    qreal progress() const;
    QString title() const;
    QUrl cover() const;
    QUrl source() const;

signals:
    void progressChanged(qreal progress);

protected:
    qreal m_progress;
    QString m_title;
    QUrl m_cover;
    QUrl m_source;
};

QML_DECLARE_TYPE(DownloadBookJob)

#endif // DOWNLOADBOOKJOB_H
