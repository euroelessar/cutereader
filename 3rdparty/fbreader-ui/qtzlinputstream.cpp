#include "qtzlinputstream.h"

QtZLInputStream::QtZLInputStream(const QString &path)
    : m_file(path)
{
}


bool QtZLInputStream::open()
{
    return m_file.open(QFile::ReadOnly);
}

size_t QtZLInputStream::read(char *buffer, size_t maxSize)
{
    return m_file.read(buffer, maxSize);
}

void QtZLInputStream::close()
{
    m_file.close();
}

void QtZLInputStream::seek(int offset, bool absoluteOffset)
{
    if (absoluteOffset) {
        m_file.seek(offset);
    } else {
        m_file.seek(m_file.pos() + offset);
    }
}

size_t QtZLInputStream::offset() const
{
    return m_file.pos();
}

size_t QtZLInputStream::sizeOfOpened()
{
    return m_file.size();
}
