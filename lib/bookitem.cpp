#include "bookitem.h"
#include <QElapsedTimer>
#include <QDebug>
#include <QMimeDatabase>
#include "bookpageitem.h"
#include "bookimageblock.h"
#include "providers/fb2imageprovider.h"
#include "formats/fb2/fb2reader.h"
#include "archivereader.h"

BookItem::BookItem(QObject *parent) :
    QObject(parent)
{
}

QList<BookBlock::Ptr> BookItem::blocks() const
{
    return m_blocks;
}

void BookItem::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_source = source;

        if (source.isLocalFile()) {
            QElapsedTimer timer;
            timer.start();
            
            ArchiveReader reader(source.toLocalFile());
            if (reader.open()) {
                QMimeDatabase mimeDataBase;
                QMimeType mime = mimeDataBase.mimeTypeForFileNameAndData(reader.fileName(), reader.device());
//                reader.device()->seek(0);
                if (mime.inherits(QStringLiteral("application/x-fictionbook"))) {
                    const QUrl baseUrl(QStringLiteral("image://fb2/") + source.toLocalFile().toUtf8().toHex());
                    
                    FB2Reader fb2Reader(baseUrl);
                    m_blocks = fb2Reader.read(reader.device());
                }
            }
            
            qDebug() << timer.restart();
        }
        emit sourceChanged(source);
    }
}

QUrl BookItem::source() const
{
    return m_source;
}

void BookItem::registerQmlTypes(QQmlEngine *engine)
{
    engine->addImageProvider("fb2", new FB2ImageProvider);
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BookPage");
}
