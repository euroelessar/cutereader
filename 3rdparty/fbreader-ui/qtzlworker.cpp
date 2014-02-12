#include "qtzlworker.h"
#include <QMutex>
#include <QWaitCondition>
#include <QImageReader>
#include <QDebug>
#include <QGuiApplication>
#include <QElapsedTimer>

#include "../fbreader/fbreader/src/formats/FormatPlugin.h"
#include "../fbreader/fbreader/src/library/Library.h"
#include "../fbreader/fbreader/src/bookmodel/BookModel.h"
#include "../fbreader/fbreader/src/database/booksdb/BooksDBUtil.h"
#include "../fbreader/fbreader/src/database/booksdb/BooksDB.h"
#include "../fbreader/fbreader/src/options/FBTextStyle.h"

#include <ZLTextHyphenator.h>
#include <ZLTextParagraph.h>
#include <ZLTextArea.h>
#include <ZLTextAreaController.h>
#include <ZLImage.h>
#include <ZLImageManager.h>

#include "qtzlimagedata.h"
#include "qtzlpaintcontext.h"
#include "qtzlnullpaintcontext.h"

class QtZLWorkerPrivate
{
public:
    shared_ptr<BookModel> model;
    QVector<shared_ptr<ZLTextModel>> bodies;
};

QtZLWorker *QtZLWorker::self = nullptr;
const QMetaMethod QtZLWorker::helperMethod = QtZLWorkerHelper::staticMetaObject.method(QtZLWorkerHelper::staticMetaObject.indexOfMethod("run(QtZLWork)"));
const QMetaMethod QtZLWorker::selfMethod = QtZLWorker::staticMetaObject.method(QtZLWorker::staticMetaObject.indexOfMethod("runInternal(QtZLWork)"));

QtZLWorker::QtZLWorker(QObject *parent) :
    QObject(parent), m_data(new QtZLWorkerPrivate)
{
    qRegisterMetaType<QtZLWork>();

    FBTextStyle::Instance().FontSizeOption.setValue(14);
    FBTextStyle::Instance().FontFamilyOption.setValue("Arial");
}

QtZLWorker::~QtZLWorker()
{
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
        sequenceInfo.number = QString::fromStdString(book->indexInSeries().value());
        result.sequences << sequenceInfo;
    }

    return result;
}

QtZLGuard QtZLWorker::loadBooks(QObject *object, const std::function<void (const QList<CuteReader::BookInfo> &)> &handler)
{
    QtZLGuard guard = QtZLGuard::create();

    run(object, guard, [handler] () {
        QList<CuteReader::BookInfo> books;
        for (const shared_ptr<Book> &book : Library::Instance().books())
            books << toBookInfo(book);
        
        qDebug() << "books" << books.size();
            
        return [books, handler] () {
            handler(books);
        };
    });

    return guard;
}

QtZLGuard QtZLWorker::openBook(QObject *object, const QString &path,
                               const std::function<void (const QtZLBookInfo &)> &handler,
                               const std::function<void (const QString &)> &error)
{
    QtZLGuard guard = QtZLGuard::create();

    run(object, guard, [this, path, handler, error] () -> QtZLWork {
        ZLFile file(path.toStdString());
        
        shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(file, false);
        if (!plugin.isNull()) {
            const QString errorStr = QString::fromStdString(plugin->tryOpen(file));
            if (!errorStr.isEmpty()) {
                return [errorStr, error] () { 
                    error(errorStr);
                };
            }
        } else {
            return [error] () { 
                error(tr("Unknown format"));
            };
        }
        
        shared_ptr<Book> book = BooksDBUtil::getBook(file.path());
        if (book.isNull()) {
            shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(file, false);
            if (!plugin.isNull()) {
                const QString errorStr = QString::fromStdString(plugin->tryOpen(file));
                if (errorStr.isEmpty()) {
                    return [errorStr, error] () { 
                        error(errorStr);
                    };
                }
            }

            return [error] () { 
                error(tr("Unknown format"));
            };
        }
        
        m_data->model = new BookModel(book);
        m_data->bodies.clear();
        m_data->bodies << m_data->model->bookTextModel();

        shared_ptr<ZLTextModel> contents = m_data->model->contentsModel();
        ContentsModel &contentsModel = static_cast<ContentsModel &>(*contents);

        for (size_t i = 0; i < contentsModel.paragraphsNumber(); ++i) {

        }

//        BooksDB::Instance().insertIntoBookList(*book);
        
        ZLTextHyphenator::Instance().load(book->language());

        QtZLBookInfo result;

        result.book = toBookInfo(book);
        
        std::deque<ReadingState> states;
        if (!BooksDB::Instance().loadBookStateStack(*book, states))
            states.push_back(ReadingState(0, 0, 0));
        
        for (const ReadingState &state : states) {
            CuteReader::BookTextPosition position = {
                0,
                state.Paragraph,
                state.Word,
                state.Character
            };

            result.positions << position;
        }

//        if (book.isNull()) {
//            info = toBookInfo(book);
//            shared_ptr<BookModel> model = new BookModel(book);
//        }

        return [result, handler] () {
            handler(result);
        };
    });

    return guard;
}

void QtZLWorker::savePositions(const QString &path, const QList<CuteReader::BookTextPosition> &positions)
{
    auto work = [path, positions] () {
        ZLFile file(path.toStdString());
        auto book = Book::loadFromFile(file);
        std::deque<ReadingState> states;
        
        for (const CuteReader::BookTextPosition &position : positions) {
            if (position.body != 0)
                continue;
            states.push_back(ReadingState(position.paragraph, position.word, position.character));
        }
        
        BooksDB::Instance().saveBookStateStack(*book, states);
    };
    
    selfMethod.invoke(this, Qt::QueuedConnection, Q_ARG(QtZLWork, work));
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
            shared_ptr<const ZLImage> image = plugin->coverImage(file);
            
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

class QtProperties : public ZLTextArea::Properties
{
public:
    shared_ptr<ZLTextStyle> baseStyle() const
    {
        return FBTextStyle::InstanceAsPtr();
    }

    ZLColor color(const std::string &style) const
    {
        static QMap<std::string, ZLColor> map;

        if (!map.contains(style)) {
            qDebug() << style.c_str();
            map.insert(style, ZLColor(0, 0, 0));
        }

        (void) style;
        return map.value(style);
    }

    bool isSelectionEnabled() const
    {
        return false;
    }
};

QtZLGuard QtZLWorker::renderPage(QObject *object, const QSize &size, const CuteReader::BookTextPosition &position,
                                 const std::function<void (const QImage &image)> &handler)
{
    QtZLGuard guard = QtZLGuard::create();

    run(object, guard, [this, handler, position, size] () {
        QImage image;
        
        shared_ptr<ZLTextModel> model = m_data->bodies.value(position.body);
        if (!model.isNull()) {
            image = QImage(size, QImage::Format_ARGB32_Premultiplied);
            image.fill(Qt::transparent);

            QtZLPaintContext context;
            context.init(image);

            QtProperties properties;

            ZLTextAreaController controller(context, properties);

            controller.setModel(model);

            controller.area().setOffsets(0, 0);
            controller.area().setSize(size.width(), size.height());

            controller.moveStartCursor(position.paragraph, position.word, position.character);
            controller.preparePaintInfo();
            controller.area().paint();
        }
        
        return [handler, image] () {
            handler(image);
        };
    });

    return guard;
}

QtZLGuard QtZLWorker::findNextPage(QObject *object, const QSize &size, const CuteReader::BookTextPosition &position,
                                   int delta, const std::function<void (const CuteReader::BookTextPosition &)> &handler)
{
    QtZLGuard guard = QtZLGuard::create();

    run(object, guard, [this, handler, position, size, delta] () {
        const CuteReader::BookTextPosition invalidResult = {
            -1, -1, -1, -1
        };
        CuteReader::BookTextPosition result = invalidResult;

        shared_ptr<ZLTextModel> model = m_data->bodies.value(position.body);
        do {
            if (model.isNull())
                break;

            QtZLNullPaintContext context;
            context.init(size);

            QtProperties properties;

            ZLTextAreaController controller(context, properties);

            controller.setModel(model);

            controller.area().setOffsets(0, 0);
            controller.area().setSize(size.width(), size.height());

            controller.moveStartCursor(position.paragraph, position.word, position.character);
            controller.preparePaintInfo();

            controller.scrollPage(delta > 0, ZLTextAreaController::NO_OVERLAPPING, std::abs(delta));
            controller.preparePaintInfo();

            const ZLTextWordCursor &cursor = controller.area().startCursor();
            result.body = position.body;
            result.paragraph = cursor.paragraphCursor().index();
            result.word = cursor.elementIndex();
            result.character = cursor.charIndex();

            if (result == position)
                result = invalidResult;
        } while (false);

        return [handler, result] {
            handler(result);
        };
    });

    return guard;
}

void QtZLWorker::run(QObject *object, const QtZLWeakGuard &guard, const std::function<QtZLWork ()> &work)
{
    if (!object) {
        selfMethod.invoke(this, Qt::QueuedConnection, Q_ARG(QtZLWork, work));
        return;
    }
    
    QtZLWorkerHelper *helper = new QtZLWorkerHelper(object);
    
    auto realWork = [helper, work, guard] () {
        if (guard) {
            QtZLWork result = work();
            helperMethod.invoke(helper, Qt::QueuedConnection, Q_ARG(QtZLWork, result));
        } else {
            QtZLWork noop = [] () {};
            helperMethod.invoke(helper, Qt::QueuedConnection, Q_ARG(QtZLWork, noop));
        }
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
