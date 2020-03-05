#include "UnitTypeModel.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <Device/DeviceModel.h>

using namespace std;

UnitTypeModel::UnitTypeModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mUnitTypes(mDb.unitTypeDao.unittypes())
{
}



QModelIndex UnitTypeModel::addUnitType(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<UnitType> newUnitType(new UnitType(0, properties));
    mDb.unitTypeDao.addUnitType(*newUnitType);
    mUnitTypes->push_back(move(newUnitType));
    endInsertRows();
    return index(rowIndex, 0);
}

int UnitTypeModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return UnitTypeColumnSize;
}

int UnitTypeModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mUnitTypes->size();
}

QVariant UnitTypeModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const UnitType& UnitType = *mUnitTypes->at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return UnitType.m_properties[UnitTypeHeaderList[index.column()]];
        case Roles::IdRole:
            return UnitType.m_properties[UnitTypeHeaderList[0]];
        default:
            return QVariant();
    }

}


bool UnitTypeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    UnitType& UnitType = *mUnitTypes->at(index.row());
//    UnitType.m_portName =  value.toString();
    mDb.unitTypeDao.updateUnitType(UnitType);
    emit dataChanged(index, index);
    return true;
}

bool UnitTypeModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const UnitType& UnitType = *mUnitTypes->at(row + countLeft);
        mDb.unitTypeDao.removeUnitType(UnitType.m_id);
    }
    mUnitTypes->erase(mUnitTypes->begin() + row, mUnitTypes->begin() + row + count);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> UnitTypeModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool UnitTypeModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
