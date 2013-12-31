#include "localbookmodel.h"
#include "frontmodel.h"
#include <algorithm>

LocalBookModel::LocalBookModel(LocalBookCollection *parent) :
    QAbstractListModel(parent), m_collection(parent)
{
    connect(parent, &LocalBookCollection::stateChanged, this, &LocalBookModel::rebuildModel);
    m_books = m_collection->books();
}

int LocalBookModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_books.size();
    return 0;
}

QVariant LocalBookModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_books.size())
        return QVariant();

    const BookInfo &info = m_books[index.row()];

    switch (role) {
    case BookTitle:
        return info.title;
    case BookSubtitle:
        return info.authorsList().join(QStringLiteral(", "));
    case BookSection:
        return info.title.at(0);
    case BookSource:
        return info.source;
    case BookCover:
        return info.cover;
    case BookAuthors:
        return info.authorsList();
    case BookGenres:
        return info.genres;
    case BookSequences:
        return info.sequencesList();
    case BookIsBook:
        return true;
    case BookActions:
        return info.actions();
    case BookData:
        return QVariant::fromValue(info.toData());
    default:
        return QVariant();
    }
}


QHash<int, QByteArray> LocalBookModel::roleNames() const
{
    return createRoleNames();
}

QHash<int, QByteArray> LocalBookModel::createRoleNames()
{
    return {
        { BookTitle, "title" },
        { BookSubtitle, "subtitle" },
        { BookSection, "section" },
        { BookSource, "source" },
        { BookCover, "coverSource" },
        { BookAuthors, "authors" },
        { BookGenres, "genres" },
        { BookSequences, "series" },
        { BookIsBook, "isBook" },
        { BookData, "data" }
    };
}

void LocalBookModel::rebuildModel()
{
    beginResetModel();
    m_books = m_collection->books();
    endResetModel();
}
