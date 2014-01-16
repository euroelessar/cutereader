#ifndef FB2IMAGEPROVIDER_H
#define FB2IMAGEPROVIDER_H

#include <QQuickImageProvider>

namespace CuteReader {

class FB2ImageProvider : public QQuickImageProvider
{
public:
    FB2ImageProvider();
    
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

} //namespace CuteReader

#endif // FB2IMAGEPROVIDER_H
