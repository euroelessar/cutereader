#ifndef BOOKROOTMODEL_H
#define BOOKROOTMODEL_H

#include <QAbstractListModel>
#include "frontmodel.h"

struct RootModelData : public ModelData
{
    RootModelData(Type type, const QString &title, const QString &subtitle, const QString &section) :
        title(title), subtitle(subtitle), section(section)
    {
        this->type = type;
    }

    QString title;
    QString subtitle;
    QString section;
};

class BookRootModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BookRootModel(QObject *parent = 0);
    ~BookRootModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QList<RootModelData> m_entries;
};

#endif // BOOKROOTMODEL_H
