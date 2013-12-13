#ifndef LOCALBOOKCOLLECTION_H
#define LOCALBOOKCOLLECTION_H

#include <QObject>
#include <QUrl>
#include <QPointer>
#include "bookinfo.h"

class LocalBookCollection : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QUrl baseDir READ baseDir WRITE setBaseDir NOTIFY baseDirChanged)
public:
    enum State {
        Null,
        Ready,
        Loading,
        Error,
        Creating
    };

    explicit LocalBookCollection(QObject *parent = 0);

    State state() const;

    QUrl baseDir() const;
    void setBaseDir(QUrl baseDir);

    void setBooks(const QUrl &baseDir, const QList<BookInfo> &books);

    void classBegin();
    void componentComplete();

signals:
    void stateChanged(State state);
    void baseDirChanged(const QUrl &baseDir);

protected:
    void loadBooks();

private:
    State m_state;
    QUrl m_baseDir;
    QList<BookInfo> m_books;
    QUrl m_cacheDir;
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
