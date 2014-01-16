#include "bookstyle.h"
#include <QStringBuilder>
#include <QQmlProperty>

namespace CuteReader {

BookStyleItem::BookStyleItem(QObject *parent) :
    QObject(parent), m_formats(TotalCount, QColor())
{
}

QVector<QColor> BookStyleItem::colors() const
{
    QReadLocker locker(&m_lock);
    return m_formats;
}

int BookStyleItem::indexForFormat(Format::Type type)
{
    static const int foregroundsIndex = staticMetaObject.indexOfEnumerator("ForegroundColorType");
    static const QMetaEnum foregrounds = staticMetaObject.enumerator(foregroundsIndex);

    static const int typesIndex = Format::staticMetaObject.indexOfEnumerator("Type");
    static const QMetaEnum types = Format::staticMetaObject.enumerator(typesIndex);

    const char *key = types.valueToKey(type);
    return foregrounds.keyToValue(key);
}

QColor BookStyleItem::value(const QString &name) const
{
    if (QColor *color = colorByName(name))
        return *color;
    return QColor();
}

void BookStyleItem::updateValue(const QString &name, const QColor &arg)
{
    if (QColor *color = colorByName(name)) {
        if (*color != arg) {
            {
                QWriteLocker lock(&m_lock);
                *color = arg;
            }
            emit changed();
        }
    }
}

QColor *BookStyleItem::colorByName(const QString &name) const
{
    QByteArray key = name.toLatin1();
    key[0] = name[0].toUpper().toLatin1();
    const QMetaObject *meta = metaObject();

    for (int i = 0; i < meta->enumeratorCount(); ++i) {
        const QMetaEnum enumerator = meta->enumerator(i);
        bool ok;
        int result = enumerator.keyToValue(key, &ok);

        if (ok) {
            return &m_formats[result];
        }
    }

    return NULL;
}

} //namespace CuteReader
