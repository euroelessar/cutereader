#include "opdsbookmodel.h"

OpdsBookModel::OpdsBookModel(QObject *parent) :
    QAbstractListModel(parent), m_state(Null)
{
}


int OpdsBookModel::rowCount(const QModelIndex &parent) const
{
}

QVariant OpdsBookModel::data(const QModelIndex &index, int role) const
{
}

void OpdsBookModel::classBegin()
{
}

void OpdsBookModel::componentComplete()
{
}
