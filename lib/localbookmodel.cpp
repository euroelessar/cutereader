#include "localbookmodel.h"

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
        { BookSource, "source" },
        { BookCover, "coverSource" },
        { BookAuthors, "authors" },
        { BookGenres, "genres" },
        { BookSequences, "series" }
    };
}

void LocalBookModel::rebuildModel()
{
    beginResetModel();
    m_books = m_collection->books();
    endResetModel();
}
