#include "bookitem.h"
#include <QElapsedTimer>
#include <QDebug>
#include <QMimeDatabase>
#include "bookpageitem.h"
#include "providers/fb2imageprovider.h"
#include "formats/fb2/fb2reader.h"
#include "archivereader.h"
#include "localbookcollection.h"
#include <QRunnable>
#include <QThreadPool>
#include <QPointer>

BookItem::BookItem(QObject *parent) :
    QObject(parent), m_state(Null), m_info(new BookInfoItem(this))
{
}

QList<BookBlockFactory::Ptr> BookItem::blocks() const
{
    return m_blocks;
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
        m_blocks.clear();
        m_source = source;
        m_state = Loading;
        emit stateChanged(m_state);
        emit sourceChanged(source);

        QThreadPool::globalInstance()->start(new BookLoader(this, m_source));
    }
}

void BookItem::setBookInfo(const BookInfo &book)
{
    if (book.source == m_source) {
        m_bookInfo = book;
        m_blocks = m_bookInfo.bodies.first().blocks;
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
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BookPage");
    qmlRegisterType<LocalBookCollection>("org.qutim", 0, 3, "LocalBookCollection");
}

BookItem::State BookItem::state() const
{
    return m_state;
}

BookInfoItem *BookItem::info() const
{
    return m_info;
}
