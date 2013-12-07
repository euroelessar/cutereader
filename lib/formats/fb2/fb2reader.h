#ifndef FB2READER_H
#define FB2READER_H

#include "../../bookblock.h"
#include "../../bookinfo.h"
#include <QMimeType>

class FB2Reader
{
public:
    enum Flag {
        Info        = 0x01,
        Text        = 0x02,
        ImageSizes  = 0x04,
        All         = 0xff
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    FB2Reader();

    bool canRead(const QMimeType &mimeType) const;
    BookInfo read(const QUrl &source, QIODevice *device, Flags flags);

private:
};

#endif // FB2READER_H
