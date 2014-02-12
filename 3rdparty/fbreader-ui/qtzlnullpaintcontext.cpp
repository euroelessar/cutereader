#include "qtzlnullpaintcontext.h"
#include "qtzlpaintcontext.h"

QtZLNullPaintContext::QtZLNullPaintContext()
{
}

void QtZLNullPaintContext::doSetFont(const QFont &font)
{
    m_font = font;
}

QFontMetrics QtZLNullPaintContext::fontMetrics() const
{
    return QFontMetrics(m_font);
}
