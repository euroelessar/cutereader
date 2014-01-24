#include "qtzlpaintcontext.h"
#include "qtzlimagedata.h"
#include <QFontDatabase>
#include <QDebug>

QtZLPaintContext::QtZLPaintContext()
{
}

void QtZLPaintContext::init(QImage &image)
{
    m_size = image.size();
    m_painter.reset(new QPainter(&image));
}

void QtZLPaintContext::deinit()
{
    m_size = QSize();
    m_painter.reset();
}

static QColor toQColor(const ZLColor &color)
{
    return QColor(color.Red, color.Green, color.Blue);
}

void QtZLPaintContext::clear(ZLColor color)
{
    m_painter->fillRect(0, 0, m_size.width(), m_size.height(), toQColor(color));
}

void QtZLPaintContext::setFont(const std::string &family, int size, bool bold, bool italic)
{
    QFont font(QString::fromStdString(family));
    font.setPixelSize(size);
    font.setBold(bold);
    font.setItalic(italic);

    m_painter->setFont(font);
}

void QtZLPaintContext::setColor(ZLColor color, ZLPaintContext::LineStyle style)
{
    QPen pen(toQColor(color), 1, style == SOLID_LINE ? Qt::SolidLine : Qt::DashLine);

    m_painter->setPen(pen);
}

void QtZLPaintContext::setFillColor(ZLColor color, ZLPaintContext::FillStyle style)
{
    QBrush brush(toQColor(color), style == SOLID_FILL ? Qt::SolidPattern : Qt::Dense4Pattern);

    m_painter->setBrush(brush);
}

int QtZLPaintContext::width() const
{
    return m_size.width();
}

int QtZLPaintContext::height() const
{
    return m_size.width();
}

int QtZLPaintContext::stringWidth(const char *str, int len, bool rtl) const
{
    (void) rtl;

    return m_painter->fontMetrics().width(QString::fromUtf8(str, len));
}

int QtZLPaintContext::spaceWidth() const
{
    return m_painter->fontMetrics().width(QLatin1Char(' '));
}

int QtZLPaintContext::stringHeight() const
{
    return m_painter->fontMetrics().height();
}

int QtZLPaintContext::descent() const
{
    return m_painter->fontMetrics().descent();
}

void QtZLPaintContext::drawString(int x, int y, const char *str, int len, bool rtl)
{
    QString string = QString::fromUtf8(str, len);
    auto layoutDirection = m_painter->layoutDirection();

    m_painter->setLayoutDirection(rtl ? Qt::RightToLeft : Qt::LeftToRight);
    m_painter->drawText(x, y, string);
    m_painter->setLayoutDirection(layoutDirection);
}

void QtZLPaintContext::drawImage(int x, int y, const ZLImageData &image)
{
    const QtZLImageData &qtData = static_cast<const QtZLImageData &>(image);
    m_painter->drawImage(x, y - qtData.height(), qtData.toImage());
}

void QtZLPaintContext::drawImage(int x, int y, const ZLImageData &image, int width, int height, ZLPaintContext::ScalingType type)
{
    const QtZLImageData &qtData = static_cast<const QtZLImageData &>(image);
    QSize size(imageWidth(image, width, height, type), imageHeight(image, width, height, type));
    QImage scaled = qtData.toImage().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_painter->drawImage(x, y - qtData.height(), scaled);
}

void QtZLPaintContext::drawLine(int x0, int y0, int x1, int y1)
{
    m_painter->drawLine(x0, y0, x1, y1);
}

void QtZLPaintContext::fillRectangle(int x0, int y0, int x1, int y1)
{
    int x = std::min(x0, x1);
    int y = std::min(y0, y1);
    int width = std::abs(x0 - x1);
    int height = std::abs(y0 - y1);

    m_painter->fillRect(x, y, width, height, m_painter->brush());
}

void QtZLPaintContext::drawFilledCircle(int x, int y, int r)
{
    m_painter->drawEllipse(x, y, 2 * r, 2 * r);
}

const std::string QtZLPaintContext::realFontFamilyName(std::string &fontFamily) const
{
    QFont font(QString::fromStdString(fontFamily));
    QFontInfo info(font);
    return info.family().toStdString();
}

void QtZLPaintContext::fillFamiliesList(std::vector<std::string> &families) const
{
    QFontDatabase database;
    for (auto family : database.families())
        families.push_back(family.toStdString());
}
