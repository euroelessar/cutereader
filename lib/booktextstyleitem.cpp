#include "booktextstyleitem.h"

BookTextStyleItem::BookTextStyleItem(const QTextCharFormat &format, QObject *parent) :
    QObject(parent), m_format(format)
{
}

QTextCharFormat BookTextStyleItem::format() const
{
    return m_format;
}

QVariant BookTextStyleItem::updateValue(QTextFormat::Property key, const QVariant &input)
{
    m_format.setProperty(key, input);
    emit changed();
    return m_format.property(key);
}
