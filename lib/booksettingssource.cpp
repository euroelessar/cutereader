#include "booksettingssource.h"
#include "booktextsettings.h"

BookSettingsSource::BookSettingsSource(QObject *parent) :
    QObject(parent)
{
}

QVariantList BookSettingsSource::formatsList() const
{
    return QVariantList();
}

QVariantMap BookSettingsSource::defaultFontSettings() const
{
    return {
        { QStringLiteral("title.fontWeight"), QFont::Bold },
        { QStringLiteral("strong.fontWeight"), QFont::Bold },
        { QStringLiteral("emphasis.fontItalic"), true },
        { QStringLiteral("strikeThrough.fontStrikeOut"), true },
        { QStringLiteral("sub.verticalAlignment"), BookTextSettings::AlignSubScript },
        { QStringLiteral("sup.verticalAlignment"), BookTextSettings::AlignSuperScript },
        { QStringLiteral("internalAnchor.underlineStyle"), int(Qt::SolidLine) },
        { QStringLiteral("externalAnchor.underlineStyle"), int(Qt::SolidLine) },
        { QStringLiteral("noteAnchor.verticalAlignment"), BookTextSettings::AlignSuperScript }
    };
}
