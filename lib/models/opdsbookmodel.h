#ifndef OPDSBOOKMODEL_H
#define OPDSBOOKMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>
#include <QUrl>
#include <QNetworkAccessManager>
#include "opdsparser.h"

class OpdsBookModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool hasNextPage READ hasNextPage NOTIFY hasNextPageChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
public:
    enum State {
        Null,
        Ready,
        Loading,
        Error,
        Creating
    };

    explicit OpdsBookModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void classBegin();
    void componentComplete();

    State state() const;

    QUrl source() const;
    void setSource(const QUrl &source);

    void load();
    void loadNext();
    void load(const QUrl &source);

    void onReplyFinished();

    bool hasNextPage() const;
    bool busy() const;

signals:
    void stateChanged(State state);
    void sourceChanged(const QUrl &source);

    void hasNextPageChanged(bool hasNextPage);
    void busyChanged(bool busy);

private:
    State m_state;
    QUrl m_source;
    OpdsInfo m_data;
    bool m_hasNextPage;
    bool m_busy;
};

#endif // OPDSBOOKMODEL_H
