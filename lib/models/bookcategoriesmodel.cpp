#include "bookcategoriesmodel.h"
#include "frontmodel.h"

BookCategoriesModel::BookCategoriesModel(QObject *parent) :
    QAbstractListModel(parent), m_type(None), m_collection(NULL)
{
}

int BookCategoriesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

QVariant BookCategoriesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.size())
        return QVariant();

    const TypeInfo &info = m_data[index.row()];

    switch (role) {
    case BookTitle:
        return info.title;
    case BookSubtitle:
        return info.subtitle;
    case BookData: {
        ModelData data;
        data.type = ModelData::LocalBooks;
        data.localFilter = info.filter;
        return QVariant::fromValue(data);
    }
    default:
        return QVariant();
    }
}

void BookCategoriesModel::rebuildModel()
{
    beginResetModel();

    const QList<BookInfo> books = m_collection ? m_collection->books() : QList<BookInfo>();
    QMap<QString, TypeInfo> categories;

    switch (m_type) {
    case None:
        m_data.clear();
        break;
    case Authors: {
        for (const BookInfo &book : books) {
            for (const QString &author : book.authorsList()) {
                TypeInfo &info = categories[author];
                info.books << book.title;

                if (info.filter.isEmpty()) {
                    info.filter = {
                        { QStringLiteral("author-match"), author }
                    };
                }
            }
        }
        break;
    }
    case Genres: {
        for (const BookInfo &book : books) {
            for (const QString &genre : book.genres) {
                TypeInfo &info = categories[genre];
                info.books << book.title;

                if (info.filter.isEmpty()) {
                    info.filter = {
                        { QStringLiteral("genre-match"), genre }
                    };
                }
            }
        }
        break;
    }
    case Series: {
        for (const BookInfo &book : books) {
            for (const SequenceInfo &sequence : book.sequences) {
                TypeInfo &info = categories[sequence.name];
                info.books << book.title;

                if (info.filter.isEmpty()) {
                    info.filter = {
                        { QStringLiteral("sequence-match"), sequence.name }
                    };
                }
            }
        }
        break;
    }
    }

    for (auto it = categories.begin(); it != categories.end(); ++it) {
        TypeInfo &info = it.value();
        std::sort(info.books.begin(), info.books.end());
        info.title = it.key();
        info.subtitle = info.books.join(QStringLiteral(", "));
        m_data << info;
    }

    endResetModel();
}

BookCategoriesModel::Type BookCategoriesModel::type() const
{
    return m_type;
}

void BookCategoriesModel::setType(BookCategoriesModel::Type type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged(type);
    }
}

LocalBookCollection *BookCategoriesModel::collection() const
{
    return m_collection;
}

void BookCategoriesModel::setCollection(LocalBookCollection *collection)
{
    if (m_collection != collection) {
        if (m_collection)
            disconnect(m_collection, 0, this, 0);

        m_collection = collection;
        emit collectionChanged(collection);

        if (collection) {
            rebuildModel();
            connect(m_collection, &LocalBookCollection::stateChanged, this, &BookCategoriesModel::rebuildModel);
        }
    }
}
