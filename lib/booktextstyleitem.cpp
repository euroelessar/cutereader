#include "booktextstyleitem.h"

BookTextStyleItem::BookTextStyleItem(const QTextCharFormat &format, QObject *parent) :
    QObject(parent), m_format(format)
{
}

QTextCharFormat BookTextStyleItem::format() const
{
    return m_format;
}
