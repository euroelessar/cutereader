#include "fontlistmodel.h"

FontListModel::FontListModel(QObject *parent) :
    QStringListModel(parent)
{
    QStringList families;

    for (const QString &family : m_db.families()) {
        const QStringList styles = m_db.styles(family);
        if (styles.isEmpty()) {
            families << family;
        } else {
            for (const QString &style : styles)
                families << QStringLiteral("%1 %2").arg(family, style);
        }
    }

    setStringList(families);
}

QVariantList FontListModel::standardSizes() const
{
    QVariantList sizes;
    for (auto size : QFontDatabase::standardSizes())
        sizes << size;
    return sizes;
}

QStringList FontListModel::families() const
{
    return stringList();
}
