#ifndef OPDSENTRYITEM_H
#define OPDSENTRYITEM_H

#include <QObject>
#include "opdsparser.h"
#include "downloadbooklist.h"
#include "frontmodel.h"

class OpdsEntryItem : public QObject
{
    Q_OBJECT
    Q_ENUMS(ActionType)
    Q_PROPERTY(QVariant entry READ entry WRITE setEntry NOTIFY entryChanged)
    Q_PROPERTY(DownloadBookList *downloadList READ downloadList WRITE setDownloadList NOTIFY downloadListChanged)
    Q_PROPERTY(QUrl cover READ cover NOTIFY entryChanged)
    Q_PROPERTY(QString title READ title NOTIFY entryChanged)
    Q_PROPERTY(QString content READ content NOTIFY entryChanged)
    Q_PROPERTY(QVariantList actions READ actions NOTIFY entryChanged)
public:
    // Keep in sync with ModelAction::Type
    enum ActionType {
        Invalid,
        OpenBook,
        OpenBookView,
        OpenView,
        OpenExternally,
        Download
    };

    explicit OpdsEntryItem(QObject *parent = 0);

    QVariant entry() const;
    QUrl cover() const;
    QString title() const;
    QString content() const;
    QVariantList actions() const;

    DownloadBookList *downloadList() const;

public slots:
    void setEntry(const QVariant &entry);
    void setDownloadList(DownloadBookList *downloadList);

signals:
    void entryChanged(const QVariant &entry);
    void downloadListChanged(DownloadBookList * arg);

private:
    ModelData m_entry;
    DownloadBookList *m_downloadList;
};

#endif // OPDSENTRYITEM_H
