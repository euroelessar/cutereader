#include "booksettingssource.h"
#include "booktextsettings.h"
#include <QFontDatabase>

namespace CuteReader {

BookSettingsSource::BookSettingsSource(QObject *parent) :
    QObject(parent)
{
}

static QVariantMap createOption(const QString &title, const QVariant &value)
{
    return {
        { QStringLiteral("title"), title },
        { QStringLiteral("value"), value }
    };
}

static QVariantMap createDefaultOption()
{
    return createOption(BookSettingsSource::tr("<Don't change>"), QVariant());
}

static QVariantMap createOption(const QString &value)
{
    return createOption(value, value);
}

QVariantList BookSettingsSource::fontFamiliesList(bool hasDefault) const
{
    QFontDatabase database;

    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    for (const QString &family : database.families()) {
        const QStringList styles = database.styles(family);
        if (styles.isEmpty()) {
            result << createOption(family);
        } else {
            for (const QString &style : styles)
                result << createOption(QStringLiteral("%1 %2").arg(family, style));
        }
    }

    return result;
}

QVariantList BookSettingsSource::fontSizesList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    for (int i = 5; i < 128; ++i)
        result << createOption(QString::number(i), i);

    return result;
}

QVariantList BookSettingsSource::fontItalicList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    result << createOption(tr("Normal style"), false);
    result << createOption(tr("Italic style"), true);

    return result;
}

QVariantList BookSettingsSource::fontBoldList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    result << createOption(tr("Normal style"), QFont::Normal);
    result << createOption(tr("Bold style"), QFont::Bold);

    return result;
}

QVariantList BookSettingsSource::fontUnderlineList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    result << createOption(tr("Turn off"), int(Qt::NoPen));
    result << createOption(tr("Turn on"), int(Qt::SolidLine));

    return result;
}

QVariantList BookSettingsSource::fontStrikeOutList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    result << createOption(tr("Turn on"), true);
    result << createOption(tr("Turn off"), false);

    return result;
}

QVariantList BookSettingsSource::fontVerticalAlignmentList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    result << createOption(tr("Normal"), BookTextSettings::AlignNormal);
    result << createOption(tr("Above the base line"), BookTextSettings::AlignSuperScript);
    result << createOption(tr("Below the base line"), BookTextSettings::AlignSubScript);

    return result;
}

QVariantList BookSettingsSource::fontAlignmentList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    result << createOption(tr("Left"), Qt::AlignLeft);
    result << createOption(tr("Right"), Qt::AlignRight);
    result << createOption(tr("Center"), Qt::AlignHCenter);
    result << createOption(tr("Justify"), Qt::AlignJustify);

    return result;
}

QVariantList BookSettingsSource::marginsList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    for (int i = 0; i < 100; ++i)
        result << createOption(QString::number(i), double(i));

    return result;
}

QVariantList BookSettingsSource::textIndentList(bool hasDefault) const
{
    QVariantList result;
    if (hasDefault)
        result << createDefaultOption();

    for (int i = 0; i < 100; ++i)
        result << createOption(QString::number(i), double(i));

    return result;
}

const QVariantMap createFormatSettings(const QString &title, const QString &configName, const QVariantList &values)
{
    return {
        { QStringLiteral("title"), title },
        { QStringLiteral("configName"), configName },
        { QStringLiteral("values"), values }
    };
}

QVariantList BookSettingsSource::formatSettingsList(const QString &type) const
{
    QVariantList result;

    const QStringList blockTypes = {
        QStringLiteral("standard"),
        QStringLiteral("title"),
        QStringLiteral("subtitle"),
        QStringLiteral("epigraph"),
        QStringLiteral("poem"),
        QStringLiteral("stanza"),
        QStringLiteral("cite"),
        QStringLiteral("textAuthor"),
        QStringLiteral("date")
    };

    const bool isBase = (type == QStringLiteral("base"));
    const bool isBlockType = blockTypes.contains(type);

    result << createFormatSettings(tr("Font family"), QStringLiteral("fontFamily"), fontFamiliesList(!isBase));
    result << createFormatSettings(tr("Font size"), QStringLiteral("fontPixelSize"), fontSizesList(!isBase));
    result << createFormatSettings(tr("Bold"), QStringLiteral("fontWeight"), fontBoldList(!isBase));
    result << createFormatSettings(tr("Italic"), QStringLiteral("fontItalic"), fontItalicList(!isBase));

    if (!isBase) {
        result << createFormatSettings(tr("Strike out"), QStringLiteral("fontStrikeOut"), fontStrikeOutList(true));
        result << createFormatSettings(tr("Underline"), QStringLiteral("underlineStyle"), fontUnderlineList(true));
        result << createFormatSettings(tr("Vertical alignment"), QStringLiteral("verticalAlignment"), fontVerticalAlignmentList(true));
    }

    if (isBase || isBlockType) {
        result << createFormatSettings(tr("Alignment"), QStringLiteral("alignment"), fontAlignmentList(!isBase));
    }

    if (isBlockType) {
        result << createFormatSettings(tr("Top margin"), QStringLiteral("topMargin"), marginsList(true));
        result << createFormatSettings(tr("Bottom margin"), QStringLiteral("bottomMargin"), marginsList(true));
        result << createFormatSettings(tr("Left margin"), QStringLiteral("leftMargin"), marginsList(true));
        result << createFormatSettings(tr("Right margin"), QStringLiteral("rightMargin"), marginsList(true));
        result << createFormatSettings(tr("First line indentation"), QStringLiteral("textIndent"), textIndentList(true));
    }

    return result;
}

static QVariantMap createFormatOption(const QString &title, const QString &type)
{
    return {
        { QStringLiteral("title"), title },
        { QStringLiteral("value"), type }
    };
}

QVariantList BookSettingsSource::formatsList() const
{
    QVariantList result;

    result << createFormatOption(tr("Standard"), QStringLiteral("standard"));
    result << createFormatOption(tr("Title"), QStringLiteral("title"));
    result << createFormatOption(tr("Subtitle"), QStringLiteral("subtitle"));
    result << createFormatOption(tr("Epigraph"), QStringLiteral("epigraph"));
    result << createFormatOption(tr("Poem"), QStringLiteral("poem"));
    result << createFormatOption(tr("Stanza"), QStringLiteral("stanza"));
    result << createFormatOption(tr("Cite"), QStringLiteral("cite"));
    result << createFormatOption(tr("Text author"), QStringLiteral("textAuthor"));
    result << createFormatOption(tr("Date"), QStringLiteral("date"));

    result << createFormatOption(tr("Strong"), QStringLiteral("strong"));
    result << createFormatOption(tr("Emphasis"), QStringLiteral("emphasis"));
    result << createFormatOption(tr("Strike through"), QStringLiteral("strikeThrough"));
    result << createFormatOption(tr("Sub"), QStringLiteral("sub"));
    result << createFormatOption(tr("Sup"), QStringLiteral("sup"));
    result << createFormatOption(tr("Internal anchor"), QStringLiteral("internalAnchor"));
    result << createFormatOption(tr("Note anchor"), QStringLiteral("noteAnchor"));
    result << createFormatOption(tr("External anchor"), QStringLiteral("externalAnchor"));
    result << createFormatOption(tr("Program code"), QStringLiteral("code"));

    return result;
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

} //namespace CuteReader
