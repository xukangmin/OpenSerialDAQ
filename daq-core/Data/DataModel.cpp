#include "DataModel.h"
#include <QDebug>

using namespace std;

DataModel::DataModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mDatas(mDb.dataDao.datas())
{
}


QModelIndex DataModel::addData(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    shared_ptr<Data> newData(new Data(0, properties));
    mDb.dataDao.addData(*newData);
    mDatas->push_back(move(newData));
    endInsertRows();
    return index(rowIndex, 0);
}

int DataModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return DataColumnSize;
}

int DataModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mDatas->size();
}


QVariant DataModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }

    const Data& data = *mDatas->at(index.row());


    switch (role) {
        case Qt::DisplayRole:
            return data.m_properties[DataHeaderList[index.column()]];
        case Roles::IdRole:
            return data.m_properties[DataHeaderList[0]];
        default:
            return QVariant();
    }

}


bool DataModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Data& data = *mDatas->at(index.row());
//    data.m_portName =  value.toString();
    mDb.dataDao.updateData(data);
    emit dataChanged(index, index);
    return true;
}

bool DataModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Data& data = *mDatas->at(row + countLeft);
        mDb.dataDao.removeData(data.m_id);
    }
    mDatas->erase(mDatas->begin() + row, mDatas->begin() + row + count);
    endRemoveRows();
    return true;

}

void DataModel::removeAllRows(const QModelIndex& parent) {
    beginRemoveRows(parent, 0, rowCount() - 1);
    mDb.dataDao.removeAll();
    mDatas->clear();
    endRemoveRows();
}


QHash<int, QByteArray> DataModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool DataModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
