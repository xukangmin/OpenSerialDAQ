#include "DeviceModel.h"
#include <QDebug>

using namespace std;

DeviceModel::DeviceModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mDevices(mDb.deviceDao.devices())
{
}

QModelIndex DeviceModel::addDevice(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Device> newDevice(new Device(0, properties));
    mDb.deviceDao.addDevice(*newDevice);
    mDevices->push_back(move(newDevice));
    endInsertRows();
    return index(rowIndex, 0);
}

void DeviceModel::addDeviceToChannel(const QModelIndex& dev_index, ChannelModel* ch_model,  const QModelIndex& ch_index)
{
    beginResetModel();
    Device& device = *mDevices->at(dev_index.row());
    QVariant channelid = ch_model->data(ch_index,Roles::IdRole);
    device.setSingleProperty("ChannelID",channelid);
    mDb.deviceDao.updateDevice(device);
    endResetModel();
}

void DeviceModel::removeDeviceFromChannel(const QModelIndex& dev_index)
{
    beginResetModel();
    Device& device = *mDevices->at(dev_index.row());
    device.setSingleProperty("ChannelID",-1);
    mDb.deviceDao.updateDevice(device);
    endResetModel();
}


int DeviceModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return DeviceColumnSize;
}

int DeviceModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mDevices->size();
}


QVariant DeviceModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const Device& device = *mDevices->at(index.row());

    qDebug() << "get data col" << index.column();

    return device.m_properties[DeviceHeaderList[index.column()]];

//    switch (role) {
//        case Qt::DisplayRole:
//            return device.m_id;
//        default:
//            return QVariant();
//    }

}


bool DeviceModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Device& device = *mDevices->at(index.row());
//    device.m_portName =  value.toString();
    mDb.deviceDao.updateDevice(device);
    emit dataChanged(index, index);
    return true;
}

bool DeviceModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Device& device = *mDevices->at(row + countLeft);
        mDb.deviceDao.removeDevice(device.m_id);
    }
    mDevices->erase(mDevices->begin() + row, mDevices->begin() + row + count);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> DeviceModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool DeviceModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
