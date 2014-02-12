#ifndef QTZLPAINTCONTEXT_H
#define QTZLPAINTCONTEXT_H

#include "qtzlbasepaintcontext.h"
#include <QPainter>

class QtZLPaintContext : public QtZLBasePaintContext
{
public:
    QtZLPaintContext();

    void init(QImage &image);
    void deinit();

    void clear(ZLColor color);
    void setColor(ZLColor color, LineStyle style);
    void setFillColor(ZLColor color, FillStyle style);
    void drawString(int x, int y, const char *str, int len, bool rtl);
    void drawImage(int x, int y, const ZLImageData &image);
    void drawImage(int x, int y, const ZLImageData &image, int width, int height, ScalingType type);
    void drawLine(int x0, int y0, int x1, int y1);
    void fillRectangle(int x0, int y0, int x1, int y1);
    void drawFilledCircle(int x, int y, int r);

protected:
    void doSetFont(const QFont &font);
    QFontMetrics fontMetrics() const;

private:
    QScopedPointer<QPainter> m_painter;
};

#endif // QTZLPAINTCONTEXT_H
