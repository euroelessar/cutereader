#include "bookitem.h"
#include <QElapsedTimer>
#include <QDebug>
#include <QMimeDatabase>
#include "bookpageitem.h"
#include "providers/fb2imageprovider.h"
#include "formats/fb2/fb2reader.h"
#include "archivereader.h"
#include "models/localbookcollection.h"
#include "models/opdsentryitem.h"
#include "models/frontmodel.h"
#include "config.h"
#include <QRunnable>
#include <QThreadPool>
#include <QPointer>

BookItem::BookItem(QObject *parent) :
    QObject(parent), m_state(Null), m_info(new BookInfoItem(this)),
    m_style(new BookStyleItem(this))
{
    connect(m_style, &BookStyleItem::changed, this, &BookItem::styleChanged);
}

QList<BookBlockFactory::Ptr> BookItem::blocks(int body) const
{
    return m_bookInfo.bodies.value(body).blocks;
}

class BookLoader : public QRunnable
{
public:
    BookLoader(BookItem *book, const QUrl &source) : m_source(source), m_book(book)
    {
    }

    void run()
    {
        if (m_source.isLocalFile()) {
            QElapsedTimer timer;
            timer.start();

            ArchiveReader reader(m_source.toLocalFile());
            if (reader.open()) {
                QMimeDatabase mimeDataBase;
                QMimeType mime = mimeDataBase.mimeTypeForFileNameAndData(reader.fileName(), reader.device());
                FB2Reader fb2Reader;
                if (fb2Reader.canRead(mime)) {
                    BookInfo book = fb2Reader.read(m_source, reader.device(), FB2Reader::All);
                    send_result(book);
                } else {
                    send_error();
                }
            } else {
                send_error();
            }

            qDebug() << timer.restart();
        } else {
            send_error();
        }
    }

    void send_result(const BookInfo &book)
    {
        QMetaObject::invokeMethod(m_book.data(), "setBookInfo", Q_ARG(BookInfo, book));
    }

    void send_error()
    {
        QMetaObject::invokeMethod(m_book.data(), "setError", Q_ARG(QUrl, m_source));
    }

private:
    QUrl m_source;
    QPointer<BookItem> m_book;
};

void BookItem::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_bookInfo = BookInfo();
        m_source = source;
        m_state = Loading;
        emit stateChanged(m_state);
        emit sourceChanged(source);

        QThreadPool::globalInstance()->start(new BookLoader(this, m_source));
    }
}

void BookItem::setConfigSource(const QUrl &configSource)
{
    if (m_configSource != configSource) {
        m_configSource = configSource;
        if (configSource.isLocalFile()) {
            Config config;
            config.loadDefaultConfig(configSource.toLocalFile());
        }
        emit configSourceChanged(configSource);
    }
}

void BookItem::setBookInfo(const BookInfo &book)
{
    if (book.source == m_source) {
        m_bookInfo = book;
        m_state = Ready;
        m_info->setBookInfo(book);
        emit stateChanged(m_state);
    }
}

void BookItem::setError(const QUrl &source)
{
    if (source == m_source) {
        m_state = Error;
        emit stateChanged(m_state);
    }
}

QUrl BookItem::source() const
{
    return m_source;
}

void BookItem::registerQmlTypes(QQmlEngine *engine)
{
    QMimeDatabase dataBase;
    Q_UNUSED(dataBase.allMimeTypes());
    qRegisterMetaType<BookInfo>();
    qRegisterMetaType<QList<BookInfo>>();
    engine->addImageProvider("fb2", new FB2ImageProvider);
    qmlRegisterUncreatableType<BookInfoItem>("org.qutim", 0, 3, "BookInfo", "This object is always Book property");
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterUncreatableType<BookStyleItem>("org.qutim", 0, 3, "BookStyle", "This object is always Book property");
    qmlRegisterUncreatableType<BookTextStyleItem>("org.qutim", 0, 3, "BookTextStyle", "This object is always Book property");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BaseBookPage");
    qmlRegisterType<LocalBookCollection>("org.qutim", 0, 3, "LocalBookCollection");
    qmlRegisterType<BookFrontModel>("org.qutim", 0, 3, "FrontModel");
    qmlRegisterType<OpdsEntryItem>("org.qutim", 0, 3, "OpdsEntry");
    qmlRegisterType<Config>("org.qutim", 0, 3, "Config");
}

BookItem::State BookItem::state() const
{
    return m_state;
}

BookInfoItem *BookItem::info() const
{
    return m_info;
}

BookTextPosition BookItem::positionForId(const QString &id) const
{
    for (int i = 0; i < m_bookInfo.bodies.size(); ++i) {
        const BodyInfo &info = m_bookInfo.bodies[i];
        auto it = info.references.find(id);
        if (it != info.references.end()) {
            return BookTextPosition {
                i,
                it->block,
                it->position
            };
        }
    }

    return BookTextPosition {
        -1, -1, -1
    };
}

BookStyle BookItem::style() const
{
    return m_style->style();
}


void BookItem::classBegin()
{
}

void BookItem::componentComplete()
{
    m_style->componentComplete();
}

QUrl BookItem::configSource() const
{
    return m_configSource;
}

BookStyleItem *BookItem::styleItem() const
{
    return m_style;
}
