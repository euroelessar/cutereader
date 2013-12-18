#include "opdsbookmodel.h"
#include <qqml.h>
#include <QQmlEngine>
#include <QNetworkReply>

enum {
    EntryTitle = Qt::UserRole,
    EntrySubtitle,
    EntryLinks,
    EntrySource,
    EntryCover,
    EntryCatalogs,
    EntryAcquisitions,
    EntryIsBook,
    EntryOpdsEntry
};

OpdsBookModel::OpdsBookModel(QObject *parent) :
    QAbstractListModel(parent), m_state(Null)
{
}


int OpdsBookModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.entries.size();
}

QVariant OpdsBookModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.entries.size())
        return QVariant();

    const OpdsEntry &entry = m_data.entries[index.row()];

    const auto catalog = QStringLiteral("application/atom+xml;profile=opds-catalog");
    const auto acquisition = QStringLiteral("http://opds-spec.org/acquisition");

    switch (role) {
    case EntryTitle:
        return entry.title;
    case EntrySubtitle:
        return entry.textContent;
    case EntryLinks: {
        QVariantList result;
        for (const OpdsLink &link : entry.links)
            result << link.toMap();
        return result;
    }
    case EntryCatalogs: {
        QVariantList result;
        for (const OpdsLink &link : entry.links) {
            if (link.type == catalog)
                result << link.toMap();
        }
        return result;
    }
    case EntrySource: {
        for (const OpdsLink &link : entry.links) {
            if (link.type == catalog && link.title.isEmpty())
                return link.source;
        }
        return QVariant();
    }
    case EntryCover:
        return entry.cover();
    case EntryAcquisitions: {
        QVariantList result;
        for (const OpdsLink &link : entry.links) {
            if (link.relation.startsWith(acquisition))
                result << link.toMap();
        }
        return result;
    }
    case EntryIsBook: {
        for (const OpdsLink &link : entry.links) {
            if (link.relation.startsWith(acquisition))
                return true;
        }
        return false;
    }
    case EntryOpdsEntry:
        return QVariant::fromValue(entry);
    default:
        return QVariant();
    }
}


QHash<int, QByteArray> OpdsBookModel::roleNames() const
{
    return {
        { EntryTitle, "title" },
        { EntrySubtitle, "subtitle" },
        { EntryLinks, "links" },
        { EntrySource, "source" },
        { EntryCatalogs, "catalogs" },
        { EntryCover, "cover" },
        { EntryAcquisitions, "acquisitions" },
        { EntryIsBook, "isBook" },
        { EntryOpdsEntry, "opdsEntry" }
    };
}

void OpdsBookModel::classBegin()
{
    m_state = Creating;
}

void OpdsBookModel::componentComplete()
{
    if (m_source.isValid())
        load();
    else
        m_state = Null;
}

OpdsBookModel::State OpdsBookModel::state() const
{
    return m_state;
}

QUrl OpdsBookModel::source() const
{
    return m_source;
}

void OpdsBookModel::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_source = source;

        if (m_state != Creating)
            load();

        emit sourceChanged(source);
    }
}

void OpdsBookModel::load()
{
    QNetworkAccessManager *manager = qmlEngine(this)->networkAccessManager();

    QNetworkRequest request(m_source);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &OpdsBookModel::onReplyFinished);
}

void OpdsBookModel::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    Q_ASSERT(reply);
    reply->deleteLater();

    beginResetModel();

    OpdsParser parser;
    m_data = parser.parse(m_source, reply);

    endResetModel();

    m_state = Ready;
}
