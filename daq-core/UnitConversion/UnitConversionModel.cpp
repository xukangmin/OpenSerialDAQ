#include "UnitConversionModel.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <Device/DeviceModel.h>

using namespace std;

UnitConversionModel::UnitConversionModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mUnitConversions(mDb.unitConversionDao.unitconversions())
{
}



QModelIndex UnitConversionModel::addUnitConversion(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<UnitConversion> newUnitConversion(new UnitConversion(0, properties));
    mDb.unitConversionDao.addUnitConversion(*newUnitConversion);
    mUnitConversions->push_back(move(newUnitConversion));
    endInsertRows();
    return index(rowIndex, 0);
}

int UnitConversionModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return UnitConversionColumnSize;
}

int UnitConversionModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mUnitConversions->size();
}

QVariant UnitConversionModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const UnitConversion& UnitConversion = *mUnitConversions->at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return UnitConversion.m_properties[UnitConversionHeaderList[index.column()]];
        case Roles::IdRole:
            return UnitConversion.m_properties[UnitConversionHeaderList[0]];
        default:
            return QVariant();
    }

}


bool UnitConversionModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    UnitConversion& UnitConversion = *mUnitConversions->at(index.row());
//    UnitConversion.m_portName =  value.toString();
    mDb.unitConversionDao.updateUnitConversion(UnitConversion);
    emit dataChanged(index, index);
    return true;
}

bool UnitConversionModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const UnitConversion& UnitConversion = *mUnitConversions->at(row + countLeft);
        mDb.unitConversionDao.removeUnitConversion(UnitConversion.m_id);
    }
    mUnitConversions->erase(mUnitConversions->begin() + row, mUnitConversions->begin() + row + count);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> UnitConversionModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool UnitConversionModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
