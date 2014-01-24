#ifndef QTZLINPUTSTREAM_H
#define QTZLINPUTSTREAM_H

#include <ZLInputStream.h>
#include <QFile>

class QtZLInputStream : public ZLInputStream
{
public:
    QtZLInputStream(const QString &path);

    bool open();
    size_t read(char *buffer, size_t maxSize);
    void close();
    void seek(int offset, bool absoluteOffset);
    size_t offset() const;
    size_t sizeOfOpened();

private:
    QFile m_file;
};

#endif // QTZLINPUTSTREAM_H
