#include "localbookcollection.h"
#include <QThreadPool>
#include <QDirIterator>
#include <QDebug>
#include <QMimeDatabase>
#include <QStringBuilder>

#include "formats/fb2/fb2reader.h"
#include "archivereader.h"

class LocalBookScanner : public QRunnable
{
public:
    LocalBookScanner(LocalBookNotifier *notifier, const QString &baseDir)
        : m_baseDir(baseDir), m_notifier(notifier)
    {
    }

    void run()
    {
        QDirIterator it(m_baseDir, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
        while (it.hasNext()) {
            it.next();

            const QFileInfo fileInfo = it.fileInfo();

            if (fileInfo.isDir())
                continue;

            const QString filePath = fileInfo.filePath();

            QFile file(filePath);
            if (!file.open(QFile::ReadOnly))
                continue;

            process(filePath, &file);
        }

        QMetaObject::invokeMethod(m_notifier, "setBooks", Qt::QueuedConnection, Q_ARG(QList<BookInfo>, m_books));
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
    QObject(parent), m_state(Null)
{
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

        if (m_baseDir.isLocalFile()) {
            m_state = Loading;
            QThreadPool::globalInstance()->start(
                        new LocalBookScanner(new LocalBookNotifier(this),
                                             m_baseDir.toLocalFile()));
        } else {
            m_state = Error;
        }

        emit stateChanged(m_state);
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
