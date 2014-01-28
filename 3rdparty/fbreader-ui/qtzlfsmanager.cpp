#include "qtzlfsmanager.h"
#include "qtzlinputstream.h"
#include "qtzloutputstream.h"
#include "qtzlfsdir.h"
#include "qtzlfsdrivesdir.h"
#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>

QtZLFSManager::QtZLFSManager()
{
}

void QtZLFSManager::createInstance()
{
    ourInstance = new QtZLFSManager;
}

static bool hasSeveralDrives()
{
#ifdef Q_OS_WIN
# error Fuck off
    return true;
#else
    return false;
#endif
}

std::string QtZLFSManager::resolveSymlink(const std::string &path) const
{
    QDir dir(QString::fromStdString(path));
    return dir.canonicalPath().toStdString();
}

void QtZLFSManager::normalizeRealPath(std::string &path) const
{
    QString oldPath = QString::fromStdString(path);
    if (oldPath.startsWith(QStringLiteral("~/")) || oldPath == QStringLiteral("~"))
        oldPath.replace(0, 1, QDir::homePath());
    
    const QFileInfo info = oldPath;
    const QDir dir = info.absolutePath();
    const QString newPath = dir.canonicalPath() + QLatin1Char('/') + info.fileName();
    path = newPath.toStdString();
}

ZLInputStream *QtZLFSManager::createPlainInputStream(const std::string &path) const
{
    return new QtZLInputStream(QString::fromStdString(path));
}

ZLOutputStream *QtZLFSManager::createOutputStream(const std::string &path) const
{
    return new QtZLOutputStream(QString::fromStdString(path));
}

ZLFSDir *QtZLFSManager::createPlainDirectory(const std::string &path) const
{
    return new QtZLFSDir(QString::fromStdString(path));
}

ZLFSDir *QtZLFSManager::createNewDirectory(const std::string &path) const
{
    QDir dir = QString::fromStdString(path);
    if (!dir.mkpath(dir.absolutePath()))
        return nullptr;

    return new QtZLFSDir(dir.absolutePath());
}

ZLFileInfo QtZLFSManager::fileInfo(const std::string &path) const
{
    const QFileInfo info = QString::fromStdString(path);

    ZLFileInfo result;
    result.Exists = info.exists();
    result.IsDirectory = info.isDir();
    result.Size = info.size();
    return result;
}

bool QtZLFSManager::removeFile(const std::string &path) const
{
    const QFileInfo info = QString::fromStdString(path);

    return info.absoluteDir().remove(info.fileName());
}

std::string QtZLFSManager::convertFilenameToUtf8(const std::string &name) const
{
    return QFile::decodeName(name.c_str()).toStdString();
}

shared_ptr<ZLMimeType> QtZLFSManager::mimeType(const std::string &path) const
{
    QMimeDatabase database;
    QMimeType type = database.mimeTypeForFile(QString::fromStdString(path));
    return ZLMimeType::get(type.name().toStdString());
}

int QtZLFSManager::findArchiveFileNameDelimiter(const std::string &path) const
{
    size_t result = path.rfind(':');
    return result == std::string::npos ? -1 : int(result);
}

shared_ptr<ZLDir> QtZLFSManager::rootDirectory() const
{
    if (hasSeveralDrives()) {
        return new QtZLFSDrivesDir;
    } else {
        return createPlainDirectory(rootDirectoryPath());
    }
}

const std::string &QtZLFSManager::rootDirectoryPath() const
{
    static const std::string path = hasSeveralDrives() ? "" : "/";
    return path;
}

std::string QtZLFSManager::parentPath(const std::string &path) const
{
    QDir dir = QString::fromStdString(path);
    if (dir.absolutePath().toStdString() == path)
        return path;
    dir.cdUp();
    return dir.absolutePath().toStdString();
}

bool QtZLFSManager::canRemoveFile(const std::string &path) const
{
    QDir dir = QString::fromStdString(path);
    return (dir.cdUp() && QFileInfo(dir.absolutePath()).isWritable());
}
