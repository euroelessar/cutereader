#include "qtzloutputstream.h"

QtZLOutputStream::QtZLOutputStream(const QString &path) : m_file(path)
{
}


bool QtZLOutputStream::open()
{
    return m_file.open(QFile::WriteOnly);
}

void QtZLOutputStream::write(const char *data, size_t len)
{
    m_file.write(data, len);
}

void QtZLOutputStream::write(const std::string &str)
{
    m_file.write(str.c_str(), str.size());
}

void QtZLOutputStream::close()
{
    m_file.close();
}
