#include "localbookcollection.h"
#include <QThreadPool>
#include <QDirIterator>
#include <QDebug>
#include <QMimeDatabase>
#include <QStringBuilder>
#include <QStandardPaths>
#include <QElapsedTimer>

#include "../formats/fb2/fb2reader.h"
#include "../archivereader.h"
#include "localbookmodel.h"

namespace CuteReader {

struct LocalBookCollectionData
{
    const static int VERSION = 4;

    struct Dir
    {
        QDateTime mtime;
        QList<BookInfo> books;
    };

    QHash<QString, Dir> directories;
};

QDataStream &operator<<(QDataStream &out, const AuthorInfo &author)
{
    out << author.firstName << author.middleName << author.lastName;
    return out;
}

QDataStream &operator>>(QDataStream &in, AuthorInfo &author)
{
    in >> author.firstName >> author.middleName >> author.lastName;
    return in;
}

QDataStream &operator<<(QDataStream &out, const SequenceInfo &sequence)
{
    out << sequence.name << sequence.number;
    return out;
}

QDataStream &operator>>(QDataStream &in, SequenceInfo &sequence)
{
    in >> sequence.name >> sequence.number;
    return in;
}

QDataStream &operator<<(QDataStream &out, const BookInfo &book)
{
    out << book.source << book.title << book.cover << book.authors << book.genres << book.sequences;
    return out;
}

QDataStream &operator>>(QDataStream &in, BookInfo &book)
{
    in >> book.source >> book.title >> book.cover >> book.authors >> book.genres >> book.sequences;
    return in;
}

QDataStream &operator<<(QDataStream &out, const LocalBookCollectionData::Dir &dir)
{
    out << dir.mtime << dir.books;
    return out;
}

QDataStream &operator>>(QDataStream &in, LocalBookCollectionData::Dir &dir)
{
    in >> dir.mtime >> dir.books;
    return in;
}

QDataStream &operator<<(QDataStream &out, const LocalBookCollectionData &data)
{
    out << data.directories;
    return out;
}

QDataStream &operator>>(QDataStream &in, LocalBookCollectionData &data)
{
    in >> data.directories;
    return in;
}


class LocalBookScanner : public QRunnable
{
public:
    LocalBookScanner(LocalBookNotifier *notifier, const QString &baseDir)
        : m_baseDir(baseDir), m_notifier(notifier)
    {
    }

    void run()
    {
        QElapsedTimer timer;
        timer.start();

        QSet<QString> listedFiles;
        QString cachePath = QStandardPaths::locate(QStandardPaths::CacheLocation, QStringLiteral("local.cache"));
        LocalBookCollectionData cachedData;
        if (!cachePath.isEmpty()) {
            QFile file(cachePath);
            if (file.open(QFile::ReadOnly)) {
                QDataStream in(&file);
                int version;
                in >> version;

                if (version == LocalBookCollectionData::VERSION)
                    in >> cachedData;
            }
        }

        bool anyChanged = false;

        QDirIterator it(m_baseDir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (it.hasNext()) {
            it.next();

            const QFileInfo fileInfo = it.fileInfo();

            if (fileInfo.isDir())
                continue;

            const QString filePath = fileInfo.canonicalFilePath();
            listedFiles << filePath;

            auto it = cachedData.directories.find(filePath);
            if (it != cachedData.directories.end() && it.value().mtime == fileInfo.lastModified())
                continue;

            QFile file(filePath);
            if (!file.open(QFile::ReadOnly))
                continue;

            process(filePath, &file);

            LocalBookCollectionData::Dir dir;
            dir.mtime = fileInfo.lastModified();
            qSwap(dir.books, m_books);

            cachedData.directories.insert(filePath, dir);
            anyChanged = true;
        }

        QList<BookInfo> books;

        QMutableHashIterator<QString, LocalBookCollectionData::Dir> jt(cachedData.directories);
        while (jt.hasNext()) {
            if (!listedFiles.contains(jt.next().key())) {
                anyChanged = true;
                jt.remove();
            } else {
                books << jt.value().books;
            }
        }

        QMetaObject::invokeMethod(m_notifier, "setBooks", Qt::QueuedConnection, Q_ARG(QList<CuteReader::BookInfo>, books));

        if (!anyChanged)
            return;

        QDir dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());

        QFile file(dir.filePath(QStringLiteral("local.cache")));
        if (file.open(QFile::WriteOnly)) {
            QDataStream out(&file);
            out << LocalBookCollectionData::VERSION << cachedData;
        } else {
            qWarning() << "Can't open" << file.fileName() << "for writing";
        }
    }

    void process(const QString &filePath, QIODevice *device)
    {
        QMimeDatabase db;
        FB2Reader reader;

        const QMimeType mimeType = db.mimeTypeForFileNameAndData(filePath, device);

        if (reader.canRead(mimeType)) {
            const QUrl source = QUrl::fromLocalFile(filePath);
            m_books << reader.read(source, device, FB2Reader::Info);
        } else {
            QScopedPointer<KArchive> archive(ArchiveReader::create(mimeType, device));
            if (archive && archive->open(QIODevice::ReadOnly)) {
                process(filePath + QLatin1Char(':'), archive->directory());
            }
        }
    }

    void process(const QString &filePath, const KArchiveDirectory *directory)
    {
        for (const QString &entryName : directory->entries()) {
            const KArchiveEntry *entry = directory->entry(entryName);
            if (entry->isFile()) {
                const KArchiveFile *file = static_cast<const KArchiveFile *>(entry);
                QScopedPointer<QIODevice> device(file->createDevice());

                const QString entryPath = filePath + entryName;
                process(entryPath, device.data());
            } else if (entry->isDirectory()) {
                const KArchiveDirectory *entryDirectory = static_cast<const KArchiveDirectory *>(entry);

                const QString entryPath = filePath % entryName % QLatin1Char('/');
                process(entryPath, entryDirectory);
            }
        }
    }

private:
    QString m_baseDir;
    QList<BookInfo> m_books;
    LocalBookNotifier *m_notifier;
};

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
        QThreadPool::globalInstance()->start(
                    new LocalBookScanner(new LocalBookNotifier(this),
                                         m_baseDir.toLocalFile()));
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
