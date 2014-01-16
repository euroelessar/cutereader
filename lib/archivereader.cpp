#include "archivereader.h"
#include "../karchive/src/kzip.h"
#include <memory>
#include <QMimeDatabase>
#include <QDebug>

namespace CuteReader {

ArchiveReader::ArchiveReader(const QString &filePath) :
    m_files(filePath.split(QLatin1Char(':'))),
    m_device(NULL)
{
}

ArchiveReader::~ArchiveReader()
{
    qDeleteAll(m_archives);
    qDeleteAll(m_objects);
}

bool ArchiveReader::open()
{
    if (m_files.isEmpty())
        return false;

    QIODevice *device = NULL;

    {
        QScopedPointer<QFile> file(new QFile(m_files.first()));
        if (!file->open(QFile::ReadOnly))
            return false;

        m_objects << file.data();
        device = file.take();
    }

    for (int i = 0; i + 1 < m_files.size(); ++i) {
        QScopedPointer<KArchive> archive(create(m_files[i], device));
        if (!archive || !archive->open(QIODevice::ReadOnly)) {
            return false;
        }

        const KArchiveEntry *entry = archive->directory()->entry(m_files[i + 1]);
        if (!entry || !entry->isFile())
            return false;

        const KArchiveFile *file = static_cast<const KArchiveFile *>(entry);

        device = file->createDevice();
        QScopedPointer<QIODevice> holder(device);
        m_archives << archive.take();
        m_objects << holder.take();
    }

    m_device = device;
    m_fileName = m_files.last();
    return true;
}

QString ArchiveReader::fileName() const
{
    return m_fileName;
}

QIODevice *ArchiveReader::device() const
{
    return m_device;
}

KArchive *ArchiveReader::create(const QString &fileName, QIODevice *data)
{
    QMimeDatabase dataBase;
    const QMimeType mimeType = dataBase.mimeTypeForFileNameAndData(fileName, data);

    return create(mimeType, data);
}

KArchive *ArchiveReader::create(const QMimeType &mimeType, QIODevice *data)
{
    if (mimeType.inherits(QStringLiteral("application/zip"))) {
        return new KZip(data);
    }

    return NULL;
}

} //namespace CuteReader
