#ifndef BOOKINFOITEM_H
#define BOOKINFOITEM_H

#include <QObject>
#include "bookinfo.h"

class BookInfoItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QUrl cover READ cover NOTIFY coverChanged)
public:
    explicit BookInfoItem(QObject *parent = 0);

    QString title() const;
    QUrl cover() const;

    void setBookInfo(const BookInfo &info);

signals:
    void titleChanged(const QString &title);
    void coverChanged(const QUrl &cover);

public slots:

private:
    QString m_title;
    QUrl m_cover;
};

#endif // BOOKINFOITEM_H
