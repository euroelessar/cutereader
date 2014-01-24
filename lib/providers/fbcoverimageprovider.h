#ifndef FBCOVERIMAGEPROVIDER_H
#define FBCOVERIMAGEPROVIDER_H

#include <QQuickImageProvider>

class FBCoverImageProvider : public QQuickImageProvider
{
public:
    FBCoverImageProvider();
    
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // FBCOVERIMAGEPROVIDER_H
