#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "device.h"
#include "devicedata.h"
#include "channel.h"

const QString DATABASE_FILENAME = "SerialDAQData.db";

class DatabaseManager
{
public:
     static DatabaseManager& instance();
     ~DatabaseManager();
    void initDevice();

    void initData();

    void initDataType();

    void initChannel();

    void init();

    void insertData(DeviceData da, int devid);


    int insertDevice(QString name, int node, QString protocol, int ch_id = 0);

    int insertChannel(Channel ch);

    void queryData();
    void removeChannel(int id) const;
    void removeDevice(int id) const;
    QVector<Device*> getAllDevice();
    QVector<Channel> getAllChannel();
protected:
    DatabaseManager(const QString& path = DATABASE_FILENAME);

private:
    void debugQuery(const QSqlQuery& query) const;
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
