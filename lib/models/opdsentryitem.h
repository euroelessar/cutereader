#ifndef OPDSENTRYITEM_H
#define OPDSENTRYITEM_H

#include <QObject>
#include "opdsparser.h"

class OpdsEntryItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant entry READ entry WRITE setEntry NOTIFY entryChanged)
    Q_PROPERTY(QUrl cover READ cover NOTIFY entryChanged)
    Q_PROPERTY(QString title READ title NOTIFY entryChanged)
    Q_PROPERTY(QString content READ content NOTIFY entryChanged)
    Q_PROPERTY(QVariantList actions READ actions NOTIFY entryChanged)
public:
    explicit OpdsEntryItem(QObject *parent = 0);

    QVariant entry() const;
    QUrl cover() const;
    QString title() const;
    QString content() const;
    QVariantList actions() const;

public slots:
    void download(const QUrl &source);

public slots:
    void setEntry(const QVariant &entry);

signals:
    void entryChanged(const QVariant &entry);

private:
    OpdsEntry m_entry;
};

#endif // OPDSENTRYITEM_H
