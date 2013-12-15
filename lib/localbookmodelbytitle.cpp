#include "localbookmodelbytitle.h"

LocalBookModelByTitle::LocalBookModelByTitle(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool LocalBookModelByTitle::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return left.data(BookTitle).toString() < right.data(BookTitle).toString();
}

QHash<int, QByteArray> LocalBookModelByTitle::roleNames() const
{
    return LocalBookModel::createRoleNames();
}
