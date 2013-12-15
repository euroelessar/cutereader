#ifndef LOCALBOOKMODEL_H
#define LOCALBOOKMODEL_H

#include <QAbstractListModel>
#include "localbookcollection.h"

enum RoleNames {
    BookTitle = Qt::UserRole,
    BookSource,
    BookCover,
    BookAuthors,
    BookGenres
};

class LocalBookModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LocalBookModel(LocalBookCollection *parent);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    static QHash<int, QByteArray> createRoleNames();

    void rebuildModel();

private:
    QList<BookInfo> m_books;
    LocalBookCollection *m_collection;
};

#endif // LOCALBOOKMODEL_H
