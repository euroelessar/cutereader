#ifndef BOOKCATEGORIESMODEL_H
#define BOOKCATEGORIESMODEL_H

#include <QAbstractListModel>
#include "localbookmodel.h"

class BookCategoriesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(LocalBookCollection *collection READ collection WRITE setCollection NOTIFY collectionChanged)
public:
    enum Type {
        None,
        Authors,
        Genres,
        Series
    };

    explicit BookCategoriesModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void rebuildModel();

    Type type() const;
    void setType(Type type);

    LocalBookCollection *collection() const;
    void setCollection(LocalBookCollection *collection);

signals:
    void typeChanged(Type type);
    void collectionChanged(LocalBookCollection *collection);

private:
    Type m_type;

    struct TypeInfo {
        QString title;
        QString subtitle;
        QStringList books;
        QVariantMap filter;

        bool operator <(const TypeInfo &other) const
        {
            return title < other.title;
        }
    };

    QList<TypeInfo> m_data;
    LocalBookCollection *m_collection;
};

#endif // BOOKCATEGORIESMODEL_H
