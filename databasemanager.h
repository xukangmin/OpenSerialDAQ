#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "device.h"
#include "devicedata.h"

const QString DATABASE_FILENAME = "SerialDAQData.db";

class DatabaseManager
{
public:
    DatabaseManager(const QString& path = DATABASE_FILENAME);
    ~DatabaseManager();

    void initDevice();

    void initData();

    void init();

    void insertData();

    int insertDevice(int node, QString protocol);

    void queryData();
    void removeDevice(int id) const;
    QVector<Device*> getAllDevice();

private:
    void debugQuery(const QSqlQuery& query) const;
    QSqlDatabase m_database;
};

#endif // DATABASEMANAGER_H
