#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include <QQmlEngine>
#include "bookblockfactory.h"
#include "bookinfoitem.h"
#include "bookinfo.h"
#include "../3rdparty/fbreader-ui/qtzlguard.h"

namespace CuteReader {

class BookTextSettings;
class BookStyleItem;

class BookItem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QUrl configSource READ configSource WRITE setConfigSource NOTIFY configSourceChanged)
    Q_PROPERTY(CuteReader::BookStyleItem *style READ styleItem WRITE setStyleItem NOTIFY styleItemChanged)
    Q_PROPERTY(CuteReader::BookInfoItem *info READ info CONSTANT FINAL)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QVariant bookData READ bookData NOTIFY bookDataChanged)
    Q_PROPERTY(QVariant contents READ contents NOTIFY contentsChanged)
    Q_PROPERTY(QVariantList positions READ positions WRITE setPositions NOTIFY positionsChanged)
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

    State state() const;

    BookInfoItem *info() const;

    BookTextPosition positionForId(const QString &id) const;

    BookStyle style() const;

    void classBegin();
    void componentComplete();

    QUrl configSource() const;
    QVariant bookData() const;
    QVariant contents() const;

    BookStyleItem *styleItem() const;
    void setStyleItem(BookStyleItem *style);

    QVariantList positions() const;

signals:
    void sourceChanged(const QUrl &source);
    void stateChanged(State state);
    void styleItemChanged(BookStyleItem *item);
    void bookDataChanged(const QVariant &bookData);
    void configSourceChanged(const QUrl &configSource);
    void positionsChanged(const QVariantList &positions);
    void contentsChanged(const QVariant &contents);

    void textSettingsChanged(const BookStyle &style);
    void styleChanged(const BookStyle &style);

public slots:
    void setSource(const QUrl &source);
    void setConfigSource(const QUrl &configSource);
    void setPositions(const QVariantList &positions);

protected slots:
    void onTextSettingsChanged();
    void onStyleChanged();
    void setBookInfo(const BookInfo &book);
    void setError(const QUrl &source);

private:
    QUrl m_source;
    State m_state;
    BookInfo m_bookInfo;
    BookInfoItem *m_info;
    int m_colorsGeneration;
    BookStyleItem *m_style;
    BookTextSettings *m_textSettings;
    QUrl m_configSource;
    QList<BookTextPosition> m_positions;
    QtZLGuard m_openGuard;
};

} //namespace CuteReader

#endif // BOOKITEM_H
