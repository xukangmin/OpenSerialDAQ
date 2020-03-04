#include "UnitModel.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <Device/DeviceModel.h>

using namespace std;

UnitModel::UnitModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mUnits(mDb.unitDao.units())
{
}



QModelIndex UnitModel::addUnit(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Unit> newUnit(new Unit(0, properties));
    mDb.unitDao.addUnit(*newUnit);
    mUnits->push_back(move(newUnit));
    endInsertRows();
    return index(rowIndex, 0);
}

int UnitModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return UnitColumnSize;
}

int UnitModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mUnits->size();
}

QVariant UnitModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const Unit& Unit = *mUnits->at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return Unit.m_properties[UnitHeaderList[index.column()]];
        case Roles::IdRole:
            return Unit.m_properties[UnitHeaderList[0]];
        default:
            return QVariant();
    }

}


bool UnitModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Unit& Unit = *mUnits->at(index.row());
//    Unit.m_portName =  value.toString();
    mDb.unitDao.updateUnit(Unit);
    emit dataChanged(index, index);
    return true;
}

bool UnitModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Unit& Unit = *mUnits->at(row + countLeft);
        mDb.unitDao.removeUnit(Unit.m_id);
    }
    mUnits->erase(mUnits->begin() + row, mUnits->begin() + row + count);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> UnitModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool UnitModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
