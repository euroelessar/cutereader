#ifndef QTZLFSDRIVESDIR_H
#define QTZLFSDRIVESDIR_H

#include "../fbreader/zlibrary/core/src/filesystem/ZLFSDir.h"

class QtZLFSDrivesDir : public ZLFSDir
{
public:
    QtZLFSDrivesDir();

    void collectSubDirs(std::vector<std::string> &names, bool includeSymlinks);
    void collectFiles(std::vector<std::string> &names, bool includeSymlinks);
};

#endif // QTZLFSDRIVESDIR_H
