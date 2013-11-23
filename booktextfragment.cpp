#include "booktextfragment.h"
#include <QFontMetricsF>

BookTextFragment::BookTextFragment()
{
}

BookTextFragment::BookTextFragment(const QFont &font, const QStringRef &text)
    : m_font(font),
      m_text(text)
{
}

BookTextFragment BookTextFragment::mid(int offset, int size)
{
    if (size == -1)
        size = m_text.size() - offset;

    return BookTextFragment(m_font, QStringRef(m_text.string(), m_text.position() + offset, size));
}
