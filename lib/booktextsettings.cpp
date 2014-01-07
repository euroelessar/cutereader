#include "booktextsettings.h"
#include <QStringBuilder>

QString fixedName(const char *str)
{
    QString result = QString::fromLatin1(str);
    result[0] = result[0].toLower();
    return result;
}

BookTextSettings::BookTextSettings(QObject *parent) :
    QObject(parent)
{
    m_config.setPath(QStringLiteral("text"));

    const QMetaObject *meta = &Format::staticMetaObject;
    const int typesIndex = meta->indexOfEnumerator("Type");
    const QMetaEnum types = meta->enumerator(typesIndex);

    const int charFormatsIndex = metaObject()->indexOfEnumerator("CharFormatType");
    const QMetaEnum charFormats = metaObject()->enumerator(charFormatsIndex);

    // skip TypesCount
    for (int i = 0; i + 1 < types.keyCount(); ++i) {
        const QString type = fixedName(types.key(i));
        QTextCharFormat &format = m_style.formats[types.value(i)];

        for (int j = 0; j < charFormats.keyCount(); ++j) {
            const QString charFormat = fixedName(charFormats.key(j));
            const QString key = (type % QLatin1Char('.') % charFormat);

            if (m_config.hasValue(key))
                format.setProperty(charFormats.value(j), m_config.value(key));
        }
    }

    connect(&m_config, &Config::valueChanged, [this, types, charFormats] (const QString &key, const QVariant &value) {
        QString type = key.section(QLatin1Char('.'), 0, 0);
        type[0] = type[0].toUpper();

        QString charFormat = key.section(QLatin1Char('.'), 1);
        charFormat[0] = charFormat[0].toUpper();

        bool okType = true, okCharFormat = true;
        const int typeIndex = types.keyToValue(type.toLatin1(), &okType);
        const int charFormatIndex = charFormats.keyToValue(charFormat.toLatin1(), &okCharFormat);

        if (!okType || !okCharFormat)
            return;

        BookStyle style;
        {
            QWriteLocker lock(&m_lock);
            QTextCharFormat &format = m_style.formats[typeIndex];
            format.setProperty(charFormatIndex, value);

            ++m_style.generation;
            style = m_style;
        }
        emit changed(m_style);
    });
}

BookStyle BookTextSettings::style() const
{
    QReadLocker locker(&m_lock);
    return m_style;
}
