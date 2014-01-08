#ifndef FRONTMODEL_H
#define FRONTMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlParserStatus>
#include "bookcategoriesmodel.h"

struct ModelAction
{
    // Keep in sync with OpdsEntryItem::ActionType
    enum Type {
        Invalid,
        OpenBook,
        OpenBookView,
        OpenView,
        OpenExternally,
        Download
    };

    ModelAction() : type(Invalid)
    {
    }

    QVariantMap toMap() const
    {
        return {
            { QStringLiteral("type"), type },
            { QStringLiteral("title"), title },
            { QStringLiteral("entry"), entry }
        };
    }

    Type type;
    QString title;
    QVariant entry;
};

struct ModelData
{
    enum Type {
        Root,
        Category,
        LocalBooks,
        Opds,
        BookView
    };

    ModelData() : type(Root), category(BookCategoriesModel::None)
    {
    }

    //All
    Type type;
    QVariantList actions;

    // Category
    BookCategoriesModel::Type category;

    // LocalBooks
    QVariantMap localFilter;

    // Opds & BookView
    QUrl source;

    // BookView
    QUrl cover;
    QString title;
    QString text;
};

class BookFrontModel : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(LocalBookCollection *collection READ collection WRITE setCollection NOTIFY collectionChanged)
    Q_PROPERTY(bool hasNextPage READ hasNextPage NOTIFY hasNextPageChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
public:
    explicit BookFrontModel(QObject *parent = 0);
    ~BookFrontModel();

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    QHash<int, QByteArray> roleNames() const;

    QVariant data() const;
    QString filter() const;
    LocalBookCollection *collection() const;

    void rebuild();

    void classBegin();
    void componentComplete();

    bool hasNextPage() const;
    bool busy() const;
    Q_INVOKABLE void loadNext();

public slots:
    void setData(const QVariant &data);
    void setFilter(const QString &filter);
    void setCollection(LocalBookCollection *collection);

signals:
    void dataChanged(const QVariant &data);
    void filterChanged(const QString &filter);
    void collectionChanged(LocalBookCollection *collection);
    void hasNextPageChanged(bool hasNextPage);
    void busyChanged(bool busy);

private:
    ModelData m_data;
    QString m_filter;
    bool m_inited;
    LocalBookCollection * m_collection;
};

Q_DECLARE_METATYPE(ModelData)

#endif // FRONTMODEL_H
