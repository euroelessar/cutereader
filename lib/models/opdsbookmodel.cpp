#include "opdsbookmodel.h"
#include "frontmodel.h"
#include <qqml.h>
#include <QQmlEngine>
#include <QNetworkReply>

namespace CuteReader {

OpdsBookModel::OpdsBookModel(QObject *parent) :
    QAbstractListModel(parent), m_state(Null), m_busy(false)
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
    case BookTitle:
        return entry.title;
    case BookSubtitle:
        return entry.textContent;
    case BookSource: {
        for (const OpdsLink &link : entry.links) {
            if (link.type == catalog && link.title.isEmpty())
                return link.source;
        }
        return QVariant();
    }
    case BookCover:
        return entry.cover();
    case BookIsBook: {
        for (const OpdsLink &link : entry.links) {
            if (link.relation.startsWith(acquisition))
                return true;
        }
        return false;
    }
    case BookData:
        return QVariant::fromValue(entry.data());
    default:
        return QVariant();
    }
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
    load(m_source);
}

void OpdsBookModel::load(const QUrl &source)
{
    QNetworkAccessManager *manager = qmlEngine(QObject::parent())->networkAccessManager();

    QNetworkRequest request(source);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &OpdsBookModel::onReplyFinished);
}

void OpdsBookModel::loadNext()
{
    const QUrl next = m_data.next();
    if (next.isValid()) {
        m_busy = true;
        emit busyChanged(m_busy);

        load(next);
    }
}

void OpdsBookModel::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    Q_ASSERT(reply);
    reply->deleteLater();

    OpdsParser parser;
    OpdsInfo data = parser.parse(reply->url(), reply);

    if (reply->url() == m_source) {
        beginResetModel();

        m_data = data;

        endResetModel();

        m_state = Ready;
    } else if (reply->url() == m_data.next()) {
        beginInsertRows(QModelIndex(), m_data.entries.size(), m_data.entries.size() + data.entries.size() - 1);

        m_data.entries.append(data.entries);
        m_data.links = data.links;

        endInsertRows();

        m_busy = false;
        emit busyChanged(m_busy);
    }

    emit hasNextPageChanged(hasNextPage());
}

bool OpdsBookModel::hasNextPage() const
{
    return m_data.next().isValid();
}

bool OpdsBookModel::busy() const
{
    return m_busy;
}

} //namespace CuteReader
