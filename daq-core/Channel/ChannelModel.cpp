#include "ChannelModel.h"
#include <QSerialPort>
#include <QSerialPortInfo>

using namespace std;

ChannelModel::ChannelModel(QObject* parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mChannels(mDb.channelDao.channels())
{
}

QVector<Channel> ChannelModel::getAllChannels()
{
    QVector<Channel> list;

    for(auto it = mChannels->begin(); it != mChannels->end(); it++) {
        list.append(*(it->get()));
    }

    return list;
}

int ChannelModel::getIndexFromID(int id)
{
    auto it = find_if(mChannels->begin(), mChannels->end(), [&id](unique_ptr<Channel>& obj) {return obj->m_id == id;});

    return distance(mChannels->begin(),it);
}

Channel ChannelModel::getChannel(int id) {
    return Channel(*mChannels->at(getIndexFromID(id)));
}

void ChannelModel::startChannel(int id)
{
    mChannels->at(getIndexFromID(id))->startChannel();
}

void ChannelModel::stopChannel(int id)
{
    mChannels->at(getIndexFromID(id))->stopChannel();
}
void ChannelModel::addDeviceToChannel(int id, int device_id)
{
     mChannels->at(getIndexFromID(id))->addDeviceToChannel(device_id);
}

QStringList ChannelModel::getAvailableComPorts() {
   QStringList list;

    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        list << port.portName();
    }

    return list;
}

bool ChannelModel::isPortExists(QString portName) {

    auto it = find_if(mChannels->begin(), mChannels->end(), [&portName](unique_ptr<Channel>& obj) {return obj->m_portName == portName;});

    if (it != mChannels->end()) {
        return true;
    } else {
        return false;
    }
}

QModelIndex ChannelModel::addChannel(Channel &channel)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Channel> newChannel(new Channel(channel));
    mDb.channelDao.addChannel(*newChannel);
    channel.m_id = (*newChannel).m_id;
    mChannels->push_back(move(newChannel));
    endInsertRows();
    return index(rowIndex, 0);
}

int ChannelModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mChannels->size();
}

void ChannelModel::removeByID(int id)
{
    removeRows(getIndexFromID(id),1);
}

QVariant ChannelModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const Channel& channel = *mChannels->at(index.row());

    switch (role) {
        case Roles::IdRole:
            return channel.m_id;
        case Roles::NameRole:
            return channel.m_channelName;
        case Roles::TypeRole:
            return channel.m_type;
        case Roles::ComPortRole:
            return channel.m_portName;
        case Roles::BaudRateRole:
            return channel.m_baudRate;
        case Roles::ParityRole:
            return channel.m_parity;
        case Roles::DataBitsRole:
            return channel.m_dataBits;
        case Roles::StopBitsRole:
            return channel.m_stopBits;
        case Qt::DisplayRole:
            return channel.m_portName;
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
    channel.m_portName =  value.toString();
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
