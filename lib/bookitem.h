#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include <QQmlEngine>
#include "bookblockfactory.h"
#include "bookinfoitem.h"
#include "bookinfo.h"
#include "bookstyle.h"

class BookItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QUrl configSource READ configSource WRITE setConfigSource NOTIFY configSourceChanged)
    Q_PROPERTY(BookStyleItem *style READ styleItem CONSTANT FINAL)
    Q_PROPERTY(BookInfoItem *info READ info CONSTANT FINAL)
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

    void classBegin();
    void componentComplete();

    QUrl configSource() const;

    BookStyleItem *styleItem() const;

signals:
    void sourceChanged(const QUrl &source);
    void stateChanged(State state);
    void styleChanged(const BookStyle &style);

    void configSourceChanged(const QUrl &configSource);

public slots:
    void setSource(const QUrl &source);

    void setConfigSource(const QUrl &configSource);

protected slots:
    void setBookInfo(const BookInfo &book);
    void setError(const QUrl &source);

private:
    QUrl m_source;
    State m_state;
    BookInfo m_bookInfo;
    BookInfoItem *m_info;
    BookStyleItem *m_style;
    QUrl m_configSource;
};

#endif // BOOKITEM_H
