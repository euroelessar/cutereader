#include "qtzlfsdrivesdir.h"
#include <QDir>

QtZLFSDrivesDir::QtZLFSDrivesDir() : ZLFSDir(std::string())
{
}

void QtZLFSDrivesDir::collectSubDirs(std::vector<std::string> &names, bool includeSymlinks)
{
    for (QFileInfo drive : QDir::drives())
        names.push_back(drive.absoluteFilePath().toStdString());

    (void) includeSymlinks;
}

void QtZLFSDrivesDir::collectFiles(std::vector<std::string> &names, bool includeSymlinks)
{
    (void) names;
    (void) includeSymlinks;
}
