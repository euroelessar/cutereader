#include "sortedlocalbookmodel.h"

SortedLocalBookModel::SortedLocalBookModel(QObject *parent) :
    QSortFilterProxyModel(parent), m_collection(NULL)
{
    sort(0);
}

bool SortedLocalBookModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    return left.data(BookTitle).toString() < right.data(BookTitle).toString();
}

bool SortedLocalBookModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    QMapIterator<QString, QVariant> it(m_filter);
    while (it.hasNext()) {
        it.next();
        int role = 0;
        QStringRef type(&it.key());
        const bool perfectMatch = type.endsWith(QStringLiteral("-match"));
        if (perfectMatch)
            type = it.key().leftRef(type.size() - 6);

        if (type == QStringLiteral("author"))
            role = BookAuthors;
        else if (type == QStringLiteral("genre"))
            role = BookGenres;
        else
            continue;

        const QStringList filters = it.value().toStringList();
        const QStringList values = index.data(role).toStringList();

        bool anyOk = false;
        for (const QString &value : values) {
            if (perfectMatch && filters.contains(value))
                anyOk = true;
            else if (!perfectMatch) {
                for (const QString &filter : filters)
                    anyOk |= filter.contains(value);
            }
        }

        if (!anyOk)
            return false;
    }

    return true;
}

QHash<int, QByteArray> SortedLocalBookModel::roleNames() const
{
    return LocalBookModel::createRoleNames();
}

LocalBookCollection *SortedLocalBookModel::collection() const
{
    return m_collection;
}

void SortedLocalBookModel::setCollection(LocalBookCollection *collection)
{
    if (m_collection != collection) {
        m_collection = collection;
        emit collectionChanged(collection);

        if (collection)
            setSourceModel(collection->model());
    }
}

QVariantMap SortedLocalBookModel::filter() const
{
    return m_filter;
}

void SortedLocalBookModel::setFilter(const QVariantMap &filter)
{
    if (m_filter != filter) {
        m_filter = filter;
        invalidate();
        emit filterChanged(filter);

    }
}
