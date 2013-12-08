#ifndef ARCHIVEREADER_H
#define ARCHIVEREADER_H

#include <QString>
#include <QFile>
#include <QMimeType>
#include "../karchive/src/karchive.h"

class ArchiveReader
{
public:
    ArchiveReader(const QString &filePath);

    ~ArchiveReader();

    bool open();

    QString fileName() const;
    QIODevice *device() const;

    static KArchive *create(const QString &fileName, QIODevice *data);
    static KArchive *create(const QMimeType &mimeType, QIODevice *data);

private:
    QStringList m_files;
    QString m_fileName;
    QIODevice *m_device;
    QList<QObject *> m_objects;
    QList<KArchive *> m_archives;
};

#endif // ARCHIVEREADER_H
