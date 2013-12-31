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
    QVariant tmp = input;

    if (key == QTextFormat::ForegroundBrush && !tmp.isNull())
        tmp = QBrush(QColor(tmp.toString()));

    m_format.setProperty(key, tmp);
    emit changed();
    return m_format.property(key);
}

QVariant BookTextStyleItem::value(QTextFormat::Property key) const
{
    if (!m_format.hasProperty(key))
        return QVariant();

    QVariant tmp = m_format.property(key);

    if (key == QTextFormat::ForegroundBrush && !tmp.isNull())
        return tmp.value<QBrush>().color().name();

    return tmp;
}
