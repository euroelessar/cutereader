#include "qtzlfsdir.h"

QtZLFSDir::QtZLFSDir(const QString &path) : ZLFSDir(path.toStdString()), m_dir(path)
{
}

void QtZLFSDir::collectSubDirs(std::vector<std::string> &names, bool includeSymlinks)
{
    QDir::Filters filters = QDir::Dirs | QDir::NoDotAndDotDot;
    if (!includeSymlinks)
        filters |= QDir::NoSymLinks;

    for (const auto &name : m_dir.entryList(filters))
        names.push_back(name.toStdString());
}

void QtZLFSDir::collectFiles(std::vector<std::string> &names, bool includeSymlinks)
{
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot;
    if (!includeSymlinks)
        filters |= QDir::NoSymLinks;

    for (const auto &name : m_dir.entryList(filters))
        names.push_back(name.toStdString());
}
