#include "qtzlbasepaintcontext.h"
#include <QFontDatabase>
#include <QFontMetrics>
#include <QFontInfo>
#include <QStringList>

QtZLBasePaintContext::QtZLBasePaintContext()
{
}

void QtZLBasePaintContext::init(const QSize &size)
{
    m_size = size;
}


void QtZLBasePaintContext::clear(ZLColor)
{
}

void QtZLBasePaintContext::setFont(const std::string &family, int size, bool bold, bool italic)
{
    QFont font(QString::fromStdString(family));
    font.setPointSize(size);
    font.setBold(bold);
    font.setItalic(italic);
    
    doSetFont(font);
}

void QtZLBasePaintContext::setColor(ZLColor, ZLPaintContext::LineStyle)
{
}

void QtZLBasePaintContext::setFillColor(ZLColor, ZLPaintContext::FillStyle)
{
}

int QtZLBasePaintContext::width() const
{
    return m_size.width();
}

int QtZLBasePaintContext::height() const
{
    return m_size.height();
}

int QtZLBasePaintContext::stringWidth(const char *str, int len, bool rtl) const
{
    (void) rtl;

    return fontMetrics().width(QString::fromUtf8(str, len));
}

int QtZLBasePaintContext::spaceWidth() const
{
    return fontMetrics().width(QLatin1Char(' '));
}

int QtZLBasePaintContext::stringHeight() const
{
    return fontMetrics().height();
}

int QtZLBasePaintContext::descent() const
{
    return fontMetrics().descent();
}

void QtZLBasePaintContext::drawString(int, int, const char *, int, bool)
{
}

void QtZLBasePaintContext::drawImage(int, int, const ZLImageData &)
{
}

void QtZLBasePaintContext::drawImage(int, int, const ZLImageData &, int, int, ZLPaintContext::ScalingType)
{
}

void QtZLBasePaintContext::drawLine(int, int, int, int)
{
}

void QtZLBasePaintContext::fillRectangle(int, int, int, int)
{
}

void QtZLBasePaintContext::drawFilledCircle(int, int, int)
{
}

const std::string QtZLBasePaintContext::realFontFamilyName(std::string &fontFamily) const
{
    QFont font(QString::fromStdString(fontFamily));
    QFontInfo info(font);
    return info.family().toStdString();
}

void QtZLBasePaintContext::fillFamiliesList(std::vector<std::string> &families) const
{
    QFontDatabase database;
    for (auto family : database.families())
        families.push_back(family.toStdString());
}
