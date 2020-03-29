#include "ChannelDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Channel.h"
#include "Database/DatabaseManager.h"
#include "Thread/ThreadChannel.h"
#include <QDebug>

using namespace std;



ChannelDao::ChannelDao(QSqlDatabase& database, QMutex& dbMutex):
    mDatabase(database), mMutex(dbMutex)
{
}

void ChannelDao::init() const
{
    QMutexLocker locker(&mMutex);
    if (!mDatabase.tables().contains("Channels")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE Channels (";

           for(int i = 0; i < ChannelColumnSize; i++) {
               createQuery += (ChannelHeaderList[i] + " " + ChannelDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void ChannelDao::addChannel(Channel& ch) const
{
    QMutexLocker locker(&mMutex);
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < ChannelColumnSize; i++) {
        insertList += (ChannelHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < ChannelColumnSize; i++) {
        dataList += (":" + ChannelHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO Channels " + insertList + " VALUES " + dataList);

    for(int i = 1; i < ChannelColumnSize; i++) {
        query.bindValue(":" + ChannelHeaderList[i], ch.getSingleProperty(ChannelHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    ch.setSingleProperty("id",ch.m_id);

    DatabaseManager::debugQuery(query);
}

void ChannelDao::updateChannel(const Channel &ch) const
{
    QMutexLocker locker(&mMutex);
}

void ChannelDao::removeChannel(int id) const {
    QMutexLocker locker(&mMutex);
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Channels WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

void ChannelDao::removeAll() const {
    QMutexLocker locker(&mMutex);
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Channels;");
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Channel>>> ChannelDao::channels() const
{
    QSqlQuery query("SELECT * FROM Channels", mDatabase);
    query.exec();
    unique_ptr<vector<unique_ptr<Channel>>> list(new vector<unique_ptr<Channel>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < ChannelColumnSize; i++) {
            properties[ChannelHeaderList[i]] = query.value(ChannelHeaderList[i]);
        }


        unique_ptr<Channel> ch(new Channel(query.value("id").toInt(),properties));

        list->push_back(move(ch));
    }
    return list;
}
