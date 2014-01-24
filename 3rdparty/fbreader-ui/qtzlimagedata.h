#ifndef QTZLIMAGEDATA_H
#define QTZLIMAGEDATA_H

#include "../fbreader/zlibrary/core/src/image/ZLImageManager.h"
#include <QImage>

class QtZLImageManager;

class QtZLImageData : public ZLImageData
{
public:
    QtZLImageData();

    unsigned int width() const;
    unsigned int height() const;
    void init(unsigned int width, unsigned int height);
    void setPosition(unsigned int x, unsigned int y);
    void moveX(int delta);
    void moveY(int delta);
    void setPixel(unsigned char r, unsigned char g, unsigned char b);
    void copyFrom(const ZLImageData &source, unsigned int targetX, unsigned int targetY);

    QImage toImage() const;

private:
    friend class QtZLImageManager;
    QImage m_image;
    QPoint m_position;
};

#endif // QTZLIMAGEDATA_H
