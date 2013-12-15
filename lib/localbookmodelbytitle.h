#ifndef LOCALBOOKMODELBYTITLE_H
#define LOCALBOOKMODELBYTITLE_H

#include <QSortFilterProxyModel>
#include "localbookmodel.h"

class LocalBookModelByTitle : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocalBookModelByTitle(QObject *parent = 0);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    QHash<int, QByteArray> roleNames() const;
};

#endif // LOCALBOOKMODELBYTITLE_H
