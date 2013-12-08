#ifndef LOCALBOOKCOLLECTION_H
#define LOCALBOOKCOLLECTION_H

#include <QObject>
#include <QUrl>
#include <QPointer>
#include "bookinfo.h"

class LocalBookCollection : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QUrl baseDir READ baseDir WRITE setBaseDir NOTIFY baseDirChanged)
public:
    enum State {
        Null,
        Ready,
        Loading,
        Error
    };

    explicit LocalBookCollection(QObject *parent = 0);

    State state() const;

    QUrl baseDir() const;
    void setBaseDir(QUrl baseDir);
    void setBooks(const QUrl &baseDir, const QList<BookInfo> &books);

signals:
    void stateChanged(State state);
    void baseDirChanged(const QUrl &baseDir);

private:
    State m_state;
    QUrl m_baseDir;
    QList<BookInfo> m_books;
};

class LocalBookNotifier : public QObject
{
    Q_OBJECT
public:
    LocalBookNotifier(LocalBookCollection *collection);

    Q_INVOKABLE void setBooks(const QList<BookInfo> &books);

private:
    QPointer<LocalBookCollection> m_collection;
    QUrl m_baseDir;
};

#endif // LOCALBOOKCOLLECTION_H
