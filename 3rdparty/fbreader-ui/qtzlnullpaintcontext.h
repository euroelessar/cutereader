#ifndef QTZLNULLPAINTCONTEXT_H
#define QTZLNULLPAINTCONTEXT_H

#include "qtzlbasepaintcontext.h"
#include <QFontMetrics>

class QtZLNullPaintContext : public QtZLBasePaintContext
{
public:
    QtZLNullPaintContext();

protected:
    void doSetFont(const QFont &font);
    QFontMetrics fontMetrics() const;
    
private:
    QFont m_font;
};

#endif // QTZLNULLPAINTCONTEXT_H
