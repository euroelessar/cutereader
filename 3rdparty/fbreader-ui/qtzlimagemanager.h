#ifndef QTZLIMAGEMANAGER_H
#define QTZLIMAGEMANAGER_H

#include "qtzlimagedata.h"

class QtZLImageManager : public ZLImageManager
{
public:
    QtZLImageManager();

    static void createInstance();

    shared_ptr<const ZLImage> makeBatchImage(const std::vector<shared_ptr<const ZLImage> > &images, shared_ptr<const ZLImage> defaultImage) const;

    shared_ptr<ZLImageData> createData() const;
    bool convertImageDirect(const std::string &stringData, ZLImageData &imageData) const;
};

#endif // QTZLIMAGEMANAGER_H
