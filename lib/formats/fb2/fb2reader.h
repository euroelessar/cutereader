#ifndef FB2READER_H
#define FB2READER_H

#include "../../bookblock.h"

class FB2Reader
{
public:
    FB2Reader(const QUrl &source);
    
    QList<BookBlock::Ptr> read(QIODevice *device);
    
private:
    QUrl m_source;
};

#endif // FB2READER_H
