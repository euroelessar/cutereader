#include "bookstyle.h"
#include <QStringBuilder>
#include <QQmlProperty>

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

void BookStyleItem::componentComplete()
{
    m_config.setPath(QStringLiteral("style.default"));
    connect(&m_config, &Config::valueChanged, [this] (const QString &key, const QVariant &value) {
        QQmlProperty property(this, key);
        if (property.isWritable())
            property.write(value);
    });

    auto meta = &BookStyleItem::staticMetaObject;
    for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
        auto property = meta->property(i);
        const auto propertyName = QString::fromLatin1(property.name());
        auto item = property.read(this).value<BookTextStyleItem*>();
        if (!item)
            continue;

        auto itemMeta = item->metaObject();
        for (int j = itemMeta->propertyOffset(); j < itemMeta->propertyCount(); ++j) {
            auto itemProperty = itemMeta->property(j);
            const auto itemPropertyName = QString::fromLatin1(itemProperty.name());
            const QString configKey = propertyName % QLatin1Char('.') % itemPropertyName;
            const QVariant value = m_config.value(configKey);
            if (value.isValid())
                itemProperty.write(item, value);
            else
                m_config.setValue(configKey, itemProperty.read(item));
        }
    }
}
