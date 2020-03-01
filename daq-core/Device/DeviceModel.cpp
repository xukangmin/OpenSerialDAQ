#include "DeviceModel.h"
#include <QDebug>

using namespace std;

DeviceModel::DeviceModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mDevices(mDb.deviceDao.devices())
{
}

QVector<Device*> DeviceModel::getAllDevices()
{
    QVector<Device*> list;

    for(auto it = mDevices->begin(); it != mDevices->end(); it++) {
        list.append(it->get());
    }

    return list;
}

int DeviceModel::getIndexFromID(int id)
{
    auto it = find_if(mDevices->begin(), mDevices->end(), [&id](unique_ptr<Device>& obj) {return obj->m_id == id;});

    return distance(mDevices->begin(),it);
}

Device* DeviceModel::getDevice(int id) {
    return (mDevices->at(getIndexFromID(id)).get());
}

bool DeviceModel::isPortExists(QString portName) {

    auto it = find_if(mDevices->begin(), mDevices->end(), [&portName](unique_ptr<Device>& obj) {return obj->getProperty("ComPort") == portName;});

    if (it != mDevices->end()) {
        return true;
    } else {
        return false;
    }
}


QModelIndex DeviceModel::addDevice(Device& device)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Device> newDevice(new Device(device.m_id, device.m_properties));
    mDb.deviceDao.addDevice(*newDevice);
    device.m_id = (*newDevice).m_id;
    mDevices->push_back(move(newDevice));
    endInsertRows();
    return index(rowIndex, 0);
}

int DeviceModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return DeviceColumnSize;
}

int DeviceModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mDevices->size();
}

void DeviceModel::removeByID(int id)
{
    removeRows(getIndexFromID(id),1);
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
