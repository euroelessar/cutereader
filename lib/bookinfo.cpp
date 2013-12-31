#include "bookinfo.h"
#include "models/frontmodel.h"
#include <QCoreApplication>

QStringList BookInfo::authorsList() const
{
    QStringList result;
    for (const AuthorInfo &author : authors)
        result << author.toString();
    std::sort(result.begin(), result.end());
    return result;
}

QStringList BookInfo::sequencesList() const
{
    QStringList result;
    for (const SequenceInfo &sequence : sequences)
        result << sequence.name;
    std::sort(result.begin(), result.end());
    return result;
}

QVariantList BookInfo::actions() const
{
    QVariantList result;
    {
        ModelAction action;
        action.type = ModelAction::OpenBook;
        action.title = QCoreApplication::translate("BookInfo", "Read");
        action.entry = source;
        result << action.toMap();
    }
    return result;
}

ModelData BookInfo::toData() const
{
    ModelData data;
    data.type = ModelData::BookView;
    data.title = title;
    data.cover = cover;
    data.source = source;
    data.text = QCoreApplication::translate("BookInfo", "Author: %1<br>\n"
                                            "Genres: %2<br>\n"
                                            "Series: %3<br>\n"
                                            "File: %4").arg(
                authorsList().join(QStringLiteral(", ")).toHtmlEscaped(),
                genres.join(QStringLiteral("/")).toHtmlEscaped(),
                sequencesList().join(QStringLiteral("/")).toHtmlEscaped(),
                source.toLocalFile().toHtmlEscaped());
    data.actions = actions();

    return data;
}

QVariantMap BookTextPosition::toMap() const
{
    if (body < 0)
        return QVariantMap();

    return {
        { QStringLiteral("body"), body },
        { QStringLiteral("block"), block },
        { QStringLiteral("position"), position }
    };
}

BookTextPosition BookTextPosition::fromMap(const QVariantMap &arg)
{
    return {
        arg.value(QStringLiteral("body"), QStringLiteral("-1")).toInt(),
        arg.value(QStringLiteral("block"), QStringLiteral("-1")).toInt(),
        arg.value(QStringLiteral("position"), QStringLiteral("-1")).toInt()
    };
}

bool BookTextPosition::operator ==(const BookTextPosition &other) const
{
    return body == other.body && block == other.block && position == other.position;
}
