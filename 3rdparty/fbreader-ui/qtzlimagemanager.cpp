#include "qtzlimagemanager.h"

QtZLImageManager::QtZLImageManager()
{
}

void QtZLImageManager::createInstance()
{
    ourInstance = new QtZLImageManager;
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
