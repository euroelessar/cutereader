#include "bookitem.h"
#include <QElapsedTimer>
#include <QDebug>
#include <QMimeDatabase>
#include "bookpageitem.h"
#include "bookimageblock.h"
#include "providers/fb2imageprovider.h"
#include "formats/fb2/fb2reader.h"
#include "archivereader.h"
#include <QRunnable>
#include <QThreadPool>
#include <QPointer>

BookItem::BookItem(QObject *parent) :
    QObject(parent), m_state(Null)
{
}

QList<BookBlock::Ptr> BookItem::blocks() const
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
//                reader.device()->seek(0);
                if (mime.inherits(QStringLiteral("application/x-fictionbook"))) {
                    const QUrl baseUrl(QStringLiteral("image://fb2/") + m_source.toLocalFile().toUtf8().toHex());

                    FB2Reader fb2Reader(baseUrl);
                    QList<BookBlock::Ptr> blocks = fb2Reader.read(reader.device());
                    send_result(blocks);
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

    void send_result(const QList<BookBlock::Ptr> &blocks)
    {
        QMetaObject::invokeMethod(m_book.data(), "setBlocks", Q_ARG(QUrl, m_source), Q_ARG(QList<BookBlock::Ptr>, blocks));
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

void BookItem::setBlocks(const QUrl &source, const QList<BookBlock::Ptr> &blocks)
{
    if (source == m_source) {
        m_blocks = blocks;
        m_state = Ready;
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
    qRegisterMetaType<QList<BookBlock::Ptr>>();
    engine->addImageProvider("fb2", new FB2ImageProvider);
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BookPage");
}

BookItem::State BookItem::state() const
{
    return m_state;
}
