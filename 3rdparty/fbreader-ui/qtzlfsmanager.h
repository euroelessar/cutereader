#ifndef QTZLFSMANAGER_H
#define QTZLFSMANAGER_H

#include "../fbreader/zlibrary/core/src/filesystem/ZLFSManager.h"

class QString;

class QtZLFSManager : public ZLFSManager
{
public:
    QtZLFSManager();

    static void createInstance();
    static QString dataPath();

public:
    std::string resolveSymlink(const std::string &path) const;

protected:
    void normalizeRealPath(std::string &path) const;
    ZLInputStream *createPlainInputStream(const std::string &path) const;
    ZLOutputStream *createOutputStream(const std::string &path) const;
    ZLFSDir *createPlainDirectory(const std::string &path) const;
    ZLFSDir *createNewDirectory(const std::string &path) const;
    ZLFileInfo fileInfo(const std::string &path) const;
    bool removeFile(const std::string &path) const;
    std::string convertFilenameToUtf8(const std::string &name) const;
    shared_ptr<ZLMimeType> mimeType(const std::string &path) const;
    int findArchiveFileNameDelimiter(const std::string &path) const;
    shared_ptr<ZLDir> rootDirectory() const;
    const std::string &rootDirectoryPath() const;
    std::string parentPath(const std::string &path) const;
    bool canRemoveFile(const std::string &path) const;
};

#endif // QTZLFSMANAGER_H
