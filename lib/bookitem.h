#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include <QQmlEngine>
#include "bookblockfactory.h"
#include "bookinfoitem.h"
#include "bookinfo.h"
#include <QReadWriteLock>

class BookItem : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(BookInfoItem *info READ info CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
public:
    enum State {
        Null,
        Ready,
        Loading,
        Error
    };

    explicit BookItem(QObject *parent = 0);

    QList<BookBlockFactory::Ptr> blocks(int body) const;

    QUrl source() const;

    static void registerQmlTypes(QQmlEngine *engine);

    State state() const;

    BookInfoItem *info() const;

    BookTextPosition positionForId(const QString &id) const;

    BookStyle style() const;
    void setStyle(const BookStyle &style);

signals:
    void sourceChanged(const QUrl &source);
    void stateChanged(State state);
    void styleChanged(const BookStyle &style);

public slots:
    void setSource(const QUrl &source);

protected slots:
    void setBookInfo(const BookInfo &book);
    void setError(const QUrl &source);

private:
    QUrl m_source;
    State m_state;
    BookInfo m_bookInfo;
    BookInfoItem *m_info;
    BookStyle m_style;
    mutable QReadWriteLock m_lock;
};

#endif // BOOKITEM_H
