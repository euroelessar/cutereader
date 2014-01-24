#include "fbcoverimageprovider.h"
#include "../../3rdparty/fbreader-ui/qtzlworker.h"

FBCoverImageProvider::FBCoverImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image,
                          QQuickImageProvider::ForceAsynchronousImageLoading)
{
}

QImage FBCoverImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = QtZLWorker::instance().loadCover(id);
    if (size)
        *size = image.size();
    return image;
}
