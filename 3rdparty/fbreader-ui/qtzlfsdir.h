#ifndef QTZLFSDIR_H
#define QTZLFSDIR_H

#include "../fbreader/zlibrary/core/src/filesystem/ZLFSDir.h"
#include <QDir>

class QtZLFSDir : public ZLFSDir
{
public:
    QtZLFSDir(const QString &path);

    void collectSubDirs(std::vector<std::string> &names, bool includeSymlinks);
    void collectFiles(std::vector<std::string> &names, bool includeSymlinks);

private:
    QDir m_dir;
};

#endif // QTZLFSDIR_H
