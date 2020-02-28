#include "ChannelDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Channel.h"
#include "Database/DatabaseManager.h"
#include "Thread/ThreadChannel.h"

using namespace std;

ChannelDao::ChannelDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void ChannelDao::init() const
{
    if (!mDatabase.tables().contains("Channels")) {
            QSqlQuery query(mDatabase);
            query.exec("CREATE TABLE Channels ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "Name TEXT, "
                               "Type TEXT, "
                               "ComPort TEXT,"
                               "BaudRate INTEGER,"
                               "Parity TEXT,"
                               "DataBits INTEGER,"
                               "StopBits INTEGER,"
                               "Source TEXT,"
                               "Port INTEGER"
                               ")");

            DatabaseManager::debugQuery(query);
        }
}

void ChannelDao::addChannel(Channel& ch) const
{
    QSqlQuery query(mDatabase);

    query.prepare("INSERT INTO Channels (Type,ComPort,BaudRate,Parity,DataBits,StopBits) VALUES (:Type,:ComPort,:BaudRate,:Parity,:DataBits,:StopBits)");

    query.bindValue(":Type",(ch.m_type == Serial ? "Serial" : "IP"));
    query.bindValue(":ComPort", ch.m_portName);
    query.bindValue(":BaudRate", ch.m_baudRate);
    query.bindValue(":Parity", ch.m_parityStr);
    query.bindValue(":DataBits", ch.m_dataBits);
    query.bindValue(":StopBits", ch.m_stopBits);
    query.exec();

    DatabaseManager::debugQuery(query);

    ch.m_id = query.lastInsertId().toInt();

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
        unique_ptr<Channel> ch(new Channel(query.value("id").toInt(),
                                            query.value("ComPort").toString(),
                                             query.value("BaudRate").toInt(),
                                             query.value("DataBits").toInt(),
                                             query.value("Parity").toString(),
                                             query.value("StopBits").toInt()));
        list->push_back(move(ch));
    }
    return list;
}
