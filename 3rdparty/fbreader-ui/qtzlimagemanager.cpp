#include "qtzlimagemanager.h"
#include <ZLImage.h>

QtZLImageManager::QtZLImageManager()
{
}

void QtZLImageManager::createInstance()
{
    ourInstance = new QtZLImageManager;
}

shared_ptr<const ZLImage> QtZLImageManager::makeBatchImage(const std::vector<shared_ptr<const ZLImage> > &images, shared_ptr<const ZLImage> defaultImage) const
{
    (void) images;
    return defaultImage;
}

shared_ptr<ZLImageData> QtZLImageManager::createData() const
{
    return new QtZLImageData;
}

bool QtZLImageManager::convertImageDirect(const std::string &stringData, ZLImageData &imageData) const
{
    QtZLImageData &qtData = static_cast<QtZLImageData &>(imageData);
    return qtData.m_image.loadFromData(reinterpret_cast<const uchar *>(stringData.c_str()), stringData.size());
}
