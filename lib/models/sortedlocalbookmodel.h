#ifndef SORTEDLOCALBOOKMODEL_H
#define SORTEDLOCALBOOKMODEL_H

#include <QSortFilterProxyModel>
#include "localbookmodel.h"

class SortedLocalBookModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(LocalBookCollection *collection READ collection WRITE setCollection NOTIFY collectionChanged)
    Q_PROPERTY(QVariantMap filter READ filter WRITE setFilter NOTIFY filterChanged)
public:
    explicit SortedLocalBookModel(QObject *parent = 0);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    QHash<int, QByteArray> roleNames() const;

    LocalBookCollection *collection() const;
    void setCollection(LocalBookCollection *collection);

    QVariantMap filter() const;
    void setFilter(const QVariantMap &filter);

signals:
    void collectionChanged(LocalBookCollection *collection);
    void filterChanged(const QVariantMap &filter);

private:
    LocalBookCollection *m_collection;
    QVariantMap m_filter;
};

#endif // SORTEDLOCALBOOKMODEL_H
