#include "ChannelDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Channel.h"
#include "Database/DatabaseManager.h"
#include "Thread/ThreadChannel.h"
#include <QDebug>

using namespace std;

ChannelDao::ChannelDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void ChannelDao::init() const
{
    if (!mDatabase.tables().contains("Channels")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE Channels (";

           qDebug() << "length=" << ChannelColumnSize;

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

    DatabaseManager::debugQuery(query);
}

void ChannelDao::updateChannel(const Channel &ch) const
{

}

void ChannelDao::removeChannel(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Channels WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Channel>>> ChannelDao::channels() const
{
    QSqlQuery query("SELECT * FROM Channels", mDatabase);
    query.exec();
    unique_ptr<vector<unique_ptr<Channel>>> list(new vector<unique_ptr<Channel>>());
    while(query.next()) {
        ;

        //        list.append(ch);
        qDebug() << "create new ch=" << query.value("id").toInt();

        QHash<QString, QVariant> properties;

        for(int i = 0; i < ChannelColumnSize; i++) {
            properties[ChannelHeaderList[i]] = query.value(ChannelHeaderList[i]);
        }


        unique_ptr<Channel> ch(new Channel(query.value("id").toInt(),properties));

        list->push_back(move(ch));
    }
    return list;
}
