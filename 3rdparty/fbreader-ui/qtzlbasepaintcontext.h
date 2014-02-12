#ifndef QTZLBASEPAINTCONTEXT_H
#define QTZLBASEPAINTCONTEXT_H

#include <ZLPaintContext.h>
#include <QFont>
#include <QSize>

class QtZLBasePaintContext : public ZLPaintContext
{
public:
    QtZLBasePaintContext();
    
    void init(const QSize &size);

    void clear(ZLColor color);
    void setFont(const std::string &family, int size, bool bold, bool italic);
    void setColor(ZLColor color, LineStyle style);
    void setFillColor(ZLColor color, FillStyle style);
    int width() const;
    int height() const;
    int stringWidth(const char *str, int len, bool rtl) const;
    int spaceWidth() const;
    int stringHeight() const;
    int descent() const;
    void drawString(int x, int y, const char *str, int len, bool rtl);
    void drawImage(int x, int y, const ZLImageData &image);
    void drawImage(int x, int y, const ZLImageData &image, int width, int height, ScalingType type);
    void drawLine(int x0, int y0, int x1, int y1);
    void fillRectangle(int x0, int y0, int x1, int y1);
    void drawFilledCircle(int x, int y, int r);
    const std::string realFontFamilyName(std::string &fontFamily) const;
    void fillFamiliesList(std::vector<std::string> &families) const;

protected:
    virtual void doSetFont(const QFont &font) = 0;
    virtual QFontMetrics fontMetrics() const = 0;

private:
    QSize m_size;
};

#endif // QTZLBASEPAINTCONTEXT_H
