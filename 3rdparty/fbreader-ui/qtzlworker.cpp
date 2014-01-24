#include "qtzlworker.h"
#include <QMutex>
#include <QWaitCondition>
#include <QImageReader>
#include <QDebug>

#include "../fbreader/fbreader/src/formats/FormatPlugin.h"
#include "../fbreader/fbreader/src/library/Library.h"
#include "../fbreader/zlibrary/core/src/image/ZLImage.h"
#include "../fbreader/zlibrary/core/src/image/ZLImageManager.h"
#include "../fbreader-ui/qtzlimagedata.h"

QtZLWorker *QtZLWorker::self = nullptr;
const QMetaMethod QtZLWorker::helperMethod = QtZLWorkerHelper::staticMetaObject.method(QtZLWorkerHelper::staticMetaObject.indexOfMethod("run(QtZLWork)"));
const QMetaMethod QtZLWorker::selfMethod = QtZLWorker::staticMetaObject.method(QtZLWorker::staticMetaObject.indexOfMethod("runInternal(QtZLWork)"));

QtZLWorker::QtZLWorker(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QtZLWork>();
}

QtZLWorker &QtZLWorker::instance()
{
    return *self;
}

void QtZLWorker::createInstance()
{
    self = new QtZLWorker;
}

void QtZLWorker::deleteInstance()
{
    delete self;
    self = nullptr;
}

static const CuteReader::BookInfo toBookInfo(const shared_ptr<Book> &book)
{
    CuteReader::BookInfo result;

    const QString path = QString::fromStdString(book->file().path());

    result.title = QString::fromStdString(book->title());
    result.source = QUrl::fromLocalFile(path);
    result.cover = QStringLiteral("image://fbcover/%1").arg(QString::fromLatin1(path.toUtf8().toBase64()));

    for (const shared_ptr<Author> &author : book->authors()) {
        CuteReader::AuthorInfo authorInfo;
        authorInfo.firstName = QString::fromStdString(author->name());
        result.authors << authorInfo;
    }

    for (const shared_ptr<Tag> &tag : book->tags()) {
        result.genres << QString::fromStdString(tag->fullName());
    }
    
    if (!book->seriesTitle().empty()) {
        CuteReader::SequenceInfo sequenceInfo;
        sequenceInfo.name = QString::fromStdString(book->seriesTitle());
        sequenceInfo.number = book->indexInSeries();
        result.sequences << sequenceInfo;
    }

    return result;
}

void QtZLWorker::loadBooks(QObject *object, const std::function<void (const QList<CuteReader::BookInfo> &)> &handler)
{
    run(object, [handler] () {
        QList<CuteReader::BookInfo> books;
        for (const shared_ptr<Book> &book : Library::Instance().books())
            books << toBookInfo(book);
        
        qDebug() << "books" << books.size();
            
        return [books, handler] () {
            handler(books);
        };
    });
}

QImage QtZLWorker::loadCover(const QString &id)
{
    QByteArray pathRaw = QByteArray::fromBase64(id.toLatin1());
    QMutex mutex;
    QWaitCondition cond;
    QImage result;
    bool ready = false;
    
    auto work = [pathRaw, &result, &ready, &mutex, &cond] () {
        ZLFile file(std::string(pathRaw.constData(), pathRaw.size()));
        shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(file, false);
        
        if (!plugin.isNull()) {
            shared_ptr<ZLImage> image = plugin->coverImage(file);
            
            if (!image.isNull()) {
                shared_ptr<ZLImageData> data = ZLImageManager::Instance().imageData(*image);
                
                if (!data.isNull())
                    result = static_cast<QtZLImageData &>(*data).toImage();
            }
        }
        
        QMutexLocker locker(&mutex);
        ready = true;
        cond.wakeAll();
    };
    
    selfMethod.invoke(this, Qt::QueuedConnection, Q_ARG(QtZLWork, work));
    
    
    QMutexLocker locker(&mutex);
    while (!ready)
        cond.wait(&mutex);
    
    return result;
}

void QtZLWorker::run(QObject *object, const std::function<QtZLWork ()> &work)
{
    QtZLWorkerHelper *helper = new QtZLWorkerHelper(object);
    
    auto realWork = [helper, work] () {
        QtZLWork result = work();
        helperMethod.invoke(helper, Qt::QueuedConnection, Q_ARG(QtZLWork, result));
    };
    
    selfMethod.invoke(this, Qt::QueuedConnection, Q_ARG(QtZLWork, realWork));
}

void QtZLWorker::runInternal(const QtZLWork &work)
{
    work();
}


QtZLWorkerHelper::QtZLWorkerHelper(QObject *object) : m_guard(object)
{
}

void QtZLWorkerHelper::run(const QtZLWork &work)
{
    if (m_guard)
        work();
    deleteLater();
}
