#include "frontmodel.h"
#include "localbookmodel.h"
#include "bookrootmodel.h"
#include "sortedlocalbookmodel.h"
#include "opdsbookmodel.h"
#include <qqml.h>
#include <QQmlEngine>

BookFrontModel::BookFrontModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

BookFrontModel::~BookFrontModel()
{
}

bool BookFrontModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_filter.isEmpty())
        return true;

    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    return index.data(BookTitle).toString().contains(m_filter, Qt::CaseInsensitive)
            || index.data(BookSubtitle).toString().contains(m_filter, Qt::CaseInsensitive);
}

QHash<int, QByteArray> BookFrontModel::roleNames() const
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
        { BookData, "entryData" },
        { BookActions, "actions" }
    };
}

QVariant BookFrontModel::data() const
{
    return QVariant::fromValue(m_data);
}

QString BookFrontModel::filter() const
{
    return m_filter;
}

LocalBookCollection *BookFrontModel::collection() const
{
    return m_collection;
}

void BookFrontModel::setData(const QVariant &data)
{
    m_data = data.value<ModelData>();
    emit dataChanged(data);

    if (m_inited)
        rebuild();
}

void BookFrontModel::setFilter(const QString &filter)
{
    if (m_filter != filter) {
        m_filter = filter;
        invalidateFilter();
        emit filterChanged(filter);
    }
}

void BookFrontModel::setCollection(LocalBookCollection *collection)
{
    if (m_collection != collection) {
        m_collection = collection;
        emit collectionChanged(collection);
    }
}

void BookFrontModel::rebuild()
{
    QPointer<QAbstractItemModel> tmpModel(sourceModel());

    switch (m_data.type) {
    case ModelData::Root:
        setSourceModel(new BookRootModel(this));
        break;
    case ModelData::Category: {
        BookCategoriesModel *model = new BookCategoriesModel(this);
        model->setType(m_data.category);
        model->setCollection(m_collection);
        setSourceModel(model);
        break;
    }
    case ModelData::LocalBooks: {
        SortedLocalBookModel *model = new SortedLocalBookModel(this);
        model->setFilter(m_data.localFilter);
        model->setCollection(m_collection);
        setSourceModel(model);
        break;
    }
    case ModelData::Opds: {
        OpdsBookModel *model = new OpdsBookModel(this);
        model->setSource(m_data.source);
        setSourceModel(model);
        break;
    }
    default:
        // TODO
        break;
    }
}

void BookFrontModel::classBegin()
{
}

void BookFrontModel::componentComplete()
{
    m_inited = true;
    rebuild();
}
