#include "booktextsettings.h"
#include <QStringBuilder>
#include <cmath>

namespace CuteReader {

static QString fixedName(const char *str)
{
    QString result = QString::fromLatin1(str);
    result[0] = result[0].toLower();
    return result;
}

static QVariant fixedValue(int propertyId, const QVariant &value)
{
    switch (propertyId) {
    case QTextFormat::FontWeight:
    case QTextFormat::TextVerticalAlignment:
    case QTextFormat::TextUnderlineStyle:
    case QTextFormat::BlockAlignment:
        return int(std::round(value.toDouble()) + 0.5);
    case QTextFormat::BlockTopMargin:
    case QTextFormat::BlockBottomMargin:
    case QTextFormat::BlockLeftMargin:
    case QTextFormat::BlockRightMargin:
    case QTextFormat::TextIndent:
        return value.toDouble();
    default:
        return value;
    };
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

    const int blockFormatsIndex = metaObject()->indexOfEnumerator("BlockFormatType");
    const QMetaEnum blockFormats = metaObject()->enumerator(blockFormatsIndex);

    // skip TypesCount
    for (int i = 0; i + 1 < types.keyCount(); ++i) {
        const QString type = fixedName(types.key(i));
        QTextCharFormat &format = m_style.formats[types.value(i)];
        QTextBlockFormat &blockFormat = m_style.blockFormats[types.value(i)];

        for (int j = 0; j < charFormats.keyCount(); ++j) {
            const QString formatName = fixedName(charFormats.key(j));
            const QString key = (type % QLatin1Char('.') % formatName);
            const int propertyId = charFormats.value(j);

            if (m_config.hasValue(key))
                format.setProperty(propertyId, fixedValue(propertyId, m_config.value(key)));
        }

        for (int j = 0; j < blockFormats.keyCount(); ++j) {
            const QString formatName = fixedName(blockFormats.key(j));
            const QString key = (type % QLatin1Char('.') % formatName);
            const int propertyId = blockFormats.value(j);

            if (m_config.hasValue(key))
                blockFormat.setProperty(propertyId, fixedValue(propertyId, m_config.value(key)));
        }
    }

    connect(&m_config, &Config::valueChanged, [this, types, charFormats, blockFormats] (const QString &key, const QVariant &value) {
        QString type = key.section(QLatin1Char('.'), 0, 0);
        type[0] = type[0].toUpper();

        QString charFormat = key.section(QLatin1Char('.'), 1);
        charFormat[0] = charFormat[0].toUpper();

        bool okType = true, okCharFormat = true, okBlockFormat = true;
        const int typeIndex = types.keyToValue(type.toLatin1(), &okType);
        const int charFormatIndex = charFormats.keyToValue(charFormat.toLatin1(), &okCharFormat);
        const int blockFormatIndex = blockFormats.keyToValue(charFormat.toLatin1(), &okBlockFormat);

        if (!okType || (!okCharFormat && !okBlockFormat))
            return;

        BookStyle style;
        {
            QWriteLocker lock(&m_lock);
            if (okCharFormat) {
                QTextCharFormat &format = m_style.formats[typeIndex];
                format.setProperty(charFormatIndex, fixedValue(charFormatIndex, value));
            } else {
                QTextBlockFormat &format = m_style.blockFormats[typeIndex];
                format.setProperty(blockFormatIndex, fixedValue(blockFormatIndex, value));
            }

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

} //namespace CuteReader
