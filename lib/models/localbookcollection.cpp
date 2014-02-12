#include "localbookcollection.h"
#include <QThreadPool>
#include <QDirIterator>
#include <QDebug>
#include <QMimeDatabase>
#include <QStringBuilder>
#include <QStandardPaths>
#include <QElapsedTimer>

#include "localbookmodel.h"
#include "../3rdparty/fbreader-ui/qtzlworker.h"

namespace CuteReader {

LocalBookCollection::LocalBookCollection(QObject *parent) :
    QObject(parent), m_state(Null), m_model(new LocalBookModel(this))
{
#ifdef Q_OS_ANDROID
    m_baseDir = QUrl::fromLocalFile("/sdcard/Books"); //TODO use jni
#else
    auto dataPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    dataPath += QStringLiteral("/books");
    m_baseDir = QUrl::fromLocalFile(dataPath);
#endif
}

LocalBookCollection::State LocalBookCollection::state() const
{
    return m_state;
}

QUrl LocalBookCollection::baseDir() const
{
    return m_baseDir;
}

void LocalBookCollection::setBaseDir(QUrl baseDir)
{
    if (m_baseDir != baseDir) {
        m_baseDir = baseDir;

        if (m_state != Creating) {
            loadBooks();
        }
        emit baseDirChanged(baseDir);
    }
}

void LocalBookCollection::setBooks(const QUrl &baseDir, const QList<BookInfo> &books)
{
    if (baseDir == m_baseDir) {
        m_books = books;
        m_state = Ready;
        emit stateChanged(m_state);
    }
}

QList<BookInfo> LocalBookCollection::books() const
{
    return m_books;
}

void LocalBookCollection::classBegin()
{
    m_state = Creating;
}

void LocalBookCollection::componentComplete()
{
    if (!m_baseDir.isEmpty())
        loadBooks();
}

QAbstractListModel *LocalBookCollection::model() const
{
    return m_model;
}

void LocalBookCollection::loadBooks()
{
    if (m_baseDir.isLocalFile()) {
        m_state = Loading;
        auto dir = m_baseDir;
        
        m_loadGuard = QtZLWorker::instance().loadBooks(this, [this, dir] (const QList<BookInfo> &books) {
            setBooks(dir, books);
        });
    } else {
        m_state = Error;
    }

    emit stateChanged(m_state);
}

LocalBookNotifier::LocalBookNotifier(LocalBookCollection *collection)
    : m_collection(collection),
      m_baseDir(collection->baseDir())
{
}

void LocalBookNotifier::setBooks(const QList<BookInfo> &books)
{
    if (m_collection)
        m_collection->setBooks(m_baseDir, books);
    deleteLater();
}

} //namespace CuteReader
