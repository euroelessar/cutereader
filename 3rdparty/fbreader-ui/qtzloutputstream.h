#ifndef QTZLOUTPUTSTREAM_H
#define QTZLOUTPUTSTREAM_H

#include <ZLOutputStream.h>
#include <QFile>

class QtZLOutputStream : public ZLOutputStream
{
public:
    QtZLOutputStream(const QString &path);

    bool open();
    void write(const char *data, size_t len);
    void write(const std::string &str);
    void close();

private:
    QFile m_file;
};

#endif // QTZLOUTPUTSTREAM_H
