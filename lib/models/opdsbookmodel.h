#ifndef OPDSBOOKMODEL_H
#define OPDSBOOKMODEL_H

#include <QAbstractListModel>
#include <QQmlParserStatus>

class OpdsBookModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_INTERFACES(QQmlParserStatus)
//    Q_PROPERTY(State state READ state NOTIFY stateChanged)
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

private:
    State m_state;
};

#endif // OPDSBOOKMODEL_H
