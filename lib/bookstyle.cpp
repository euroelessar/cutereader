#include "bookstyle.h"

BookStyleItem::BookStyleItem(QObject *parent) :
    QObject(parent), m_formats(Format::TypesCount), m_style(BookStyle::defaultStyle())
{
    for (int i = 0; i < Format::TypesCount; ++i) {
        auto format = new BookTextStyleItem(m_style.formats[i], this);
        m_formats[i] = format;
        connect(m_formats[i], &BookTextStyleItem::changed, [this, format, i] {
            QWriteLocker locker(&m_lock);
            ++m_style.generation;
            m_style.formats[i] = format->format();
            auto style = m_style;
            locker.unlock();

            emit changed(style);
        });
    }
}

BookStyle BookStyleItem::style() const
{
    QReadLocker locker(&m_lock);
    return m_style;
}
