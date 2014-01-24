#include "qtzlimagedata.h"
#include <QPainter>

QtZLImageData::QtZLImageData()
{
}

unsigned int QtZLImageData::width() const
{
    return m_image.width();
}

unsigned int QtZLImageData::height() const
{
    return m_image.height();
}

void QtZLImageData::init(unsigned int width, unsigned int height)
{
    m_image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
}

void QtZLImageData::setPosition(unsigned int x, unsigned int y)
{
    m_position = QPoint(x, y);
}

void QtZLImageData::moveX(int delta)
{
    m_position.rx() += delta;
}

void QtZLImageData::moveY(int delta)
{
    m_position.ry() += delta;
}

void QtZLImageData::setPixel(unsigned char r, unsigned char g, unsigned char b)
{
    m_image.setPixel(m_position, qRgb(r, g, b));
}

void QtZLImageData::copyFrom(const ZLImageData &source, unsigned int targetX, unsigned int targetY)
{
    const QImage other = static_cast<const QtZLImageData &>(source).toImage();

    if (other.isNull() || m_image.isNull())
        return;

    QPainter painter(&m_image);
    painter.drawImage(targetX, targetY, other);
}

QImage QtZLImageData::toImage() const
{
    return m_image;
}
