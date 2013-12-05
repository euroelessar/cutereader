#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include <QQmlEngine>
#include "booktextblock.h"

class BookItem : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
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

signals:
    void sourceChanged(const QUrl &source);
    void stateChanged(State state);

public slots:
    void setSource(const QUrl &source);

protected slots:
    void setBlocks(const QUrl &source, const QList<BookBlock::Ptr> &blocks);
    void setError(const QUrl &source);

private:
    QList<BookBlock::Ptr> m_blocks;
    QUrl m_source;
    State m_state;
};

#endif // BOOKITEM_H
