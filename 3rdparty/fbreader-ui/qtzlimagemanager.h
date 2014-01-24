#ifndef QTZLIMAGEMANAGER_H
#define QTZLIMAGEMANAGER_H

#include "qtzlimagedata.h"

class QtZLImageManager : public ZLImageManager
{
public:
    QtZLImageManager();

    static void createInstance();

    shared_ptr<ZLImageData> createData() const;
    bool convertImageDirect(const std::string &stringData, ZLImageData &imageData) const;
};

#endif // QTZLIMAGEMANAGER_H
