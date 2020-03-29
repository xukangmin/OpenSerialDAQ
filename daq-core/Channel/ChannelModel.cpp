#include "ChannelModel.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <Device/DeviceModel.h>

using namespace std;

ChannelModel::ChannelModel(QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mChannels(mDb.channelDao.channels())
{
}

QList<QString> ChannelModel::getAvailablePorts() {

    QList<QString> tmp;

    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        tmp.append(port.portName());
    }

    return tmp;
}

//QVector<Channel*> ChannelModel::getAllChannels()
//{
//    QVector<Channel*> list;

//    for(auto it = mChannels->begin(); it != mChannels->end(); it++) {
//        list.append(it->get());
//    }

//    return list;
//}

//int ChannelModel::getIndexFromID(int id)
//{
//    auto it = find_if(mChannels->begin(), mChannels->end(), [&id](unique_ptr<Channel>& obj) {return obj->m_id == id;});

//    return distance(mChannels->begin(),it);
//}

//Channel* ChannelModel::getChannel(int id) {
//    return (mChannels->at(getIndexFromID(id)).get());
//}

bool ChannelModel::isPortExists(QString portName) {

    auto it = find_if(mChannels->begin(), mChannels->end(), [&portName](unique_ptr<Channel>& obj) {return obj->getSingleProperty("ComPort") == portName;});

    if (it != mChannels->end()) {
        return true;
    } else {
        return false;
    }
}


void ChannelModel::addDeviceToChannel(const shared_ptr<Device>& dev, const QModelIndex& ch_index) {
    Channel& channel = *mChannels->at(ch_index.row());
    channel.addDeviceToThread(dev);
}

void ChannelModel::removeDeviceFromChannel(const shared_ptr<Device>& dev, const QModelIndex& ch_index) {
    Channel& channel = *mChannels->at(ch_index.row());
    channel.removeDeviceFromThread(dev);
}


QModelIndex ChannelModel::addChannel(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Channel> newChannel(new Channel(0, properties));
    mDb.channelDao.addChannel(*newChannel);

    mChannels->push_back(move(newChannel));
    endInsertRows();
    return index(rowIndex, 0);
}

void ChannelModel::startChannel(const QModelIndex& index) {
    Channel& channel = *mChannels->at(index.row());

    channel.startChannel();
}

void ChannelModel::stopChannel(const QModelIndex& index){
    Channel& channel = *mChannels->at(index.row());

    channel.stopChannel();
}


int ChannelModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return ChannelColumnSize;
}

int ChannelModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mChannels->size();
}

QVariant ChannelModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const Channel& channel = *mChannels->at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return channel.m_properties[ChannelHeaderList[index.column()]];
        case Roles::IdRole:
            return channel.m_properties[ChannelHeaderList[0]];
        default:
            return QVariant();
    }

}


bool ChannelModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Channel& channel = *mChannels->at(index.row());
//    channel.m_portName =  value.toString();
    mDb.channelDao.updateChannel(channel);
    emit dataChanged(index, index);
    return true;
}

bool ChannelModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Channel& channel = *mChannels->at(row + countLeft);
        mDb.channelDao.removeChannel(channel.m_id);
    }
    mChannels->erase(mChannels->begin() + row, mChannels->begin() + row + count);
    endRemoveRows();
    return true;

}

void ChannelModel::removeAllRows(const QModelIndex& parent) {
    beginRemoveRows(parent, 0, rowCount() - 1);
    mDb.channelDao.removeAll();
    mChannels->clear();
    endRemoveRows();
}

QHash<int, QByteArray> ChannelModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool ChannelModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
