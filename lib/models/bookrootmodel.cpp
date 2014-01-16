#include "bookrootmodel.h"
#include "localbookmodel.h"

namespace CuteReader {

BookRootModel::BookRootModel(QObject *parent) :
    QAbstractListModel(parent)
{
    {
        RootModelData data(ModelData::Category, tr("Authors"), tr("Books categorized by authors"), tr("Local"));
        data.category = BookCategoriesModel::Authors;
        m_entries << data;
    }
    {
        RootModelData data(ModelData::Category, tr("Genres"), tr("Books categorized by genres"), tr("Local"));
        data.category = BookCategoriesModel::Genres;
        m_entries << data;
    }
    {
        RootModelData data(ModelData::Category, tr("Series"), tr("Books categorized by series"), tr("Local"));
        data.category = BookCategoriesModel::Series;
        m_entries << data;
    }
    {
        RootModelData data(ModelData::LocalBooks, tr("Titles"), tr("Books categorized by titles"), tr("Local"));
        m_entries << data;
    }
    {
        RootModelData data(ModelData::LocalBooks, tr("Recent"), tr("Recently opened books"), tr("Local"));
        m_entries << data;
    }
    {
        RootModelData data(ModelData::Opds, tr("Opds"), tr("Network library"), tr("Network"));
        data.source = QUrl(QStringLiteral("http://flibusta.net/opds/author/19011/alphabet/1"));
        m_entries << data;
    }
}

BookRootModel::~BookRootModel()
{
}

int BookRootModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_entries.size();
}

QVariant BookRootModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_entries.size())
        return QVariant();

    const RootModelData &data = m_entries[index.row()];

    switch (role) {
    case BookTitle:
        return data.title;
    case BookSubtitle:
        return data.subtitle;
    case BookSection:
        return data.section;
    case BookData:
        return QVariant::fromValue<ModelData>(data);
    default:
        return QVariant();
    }
}

} //namespace CuteReader
