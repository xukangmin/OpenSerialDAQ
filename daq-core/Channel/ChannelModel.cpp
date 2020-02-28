#include "ChannelModel.h"

using namespace std;

ChannelModel::ChannelModel(QObject* parent) :
    QAbstractListModel(parent),
    mDb(DatabaseManager::instance()),
    mChannels(mDb.channelDao.channels())
{
}

QModelIndex ChannelModel::addChannel(const Channel &channel)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Channel> newChannel(new Channel(channel));
    mDb.channelDao.addChannel(*newChannel);
    mChannels->push_back(move(newChannel));
    endInsertRows();
    return index(rowIndex, 0);
}
