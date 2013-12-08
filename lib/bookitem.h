#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include <QQmlEngine>
#include "booktextblock.h"
#include "bookinfoitem.h"
#include "bookinfo.h"

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

    QList<BookBlock::Ptr> blocks() const;

    QUrl source() const;

    static void registerQmlTypes(QQmlEngine *engine);

    State state() const;

    BookInfoItem *info() const;

signals:
    void sourceChanged(const QUrl &source);
    void stateChanged(State state);

public slots:
    void setSource(const QUrl &source);

protected slots:
    void setBookInfo(const BookInfo &book);
    void setError(const QUrl &source);

private:
    QList<BookBlock::Ptr> m_blocks;
    QUrl m_source;
    State m_state;
    BookInfo m_bookInfo;
    BookInfoItem *m_info;
};

#endif // BOOKITEM_H
