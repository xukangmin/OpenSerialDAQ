#include "DeviceDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Device.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

DeviceDao::DeviceDao(QSqlDatabase& database, QMutex& dbMutex):
    mDatabase(database), mMutex(dbMutex)
{
}

void DeviceDao::init() const
{
    QMutexLocker locker(&mMutex);

    if (!mDatabase.tables().contains("Devices")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE Devices (";

           qDebug() << "length=" << DeviceColumnSize;

           for(int i = 0; i < DeviceColumnSize; i++) {
               createQuery += (DeviceHeaderList[i] + " " + DeviceDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void DeviceDao::addDevice(Device& ch) const
{
    QMutexLocker locker(&mMutex);

    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < DeviceColumnSize; i++) {
        insertList += (DeviceHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < DeviceColumnSize; i++) {
        dataList += (":" + DeviceHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO Devices " + insertList + " VALUES " + dataList);

    for(int i = 1; i < DeviceColumnSize; i++) {
        query.bindValue(":" + DeviceHeaderList[i], ch.getSingleProperty(DeviceHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    ch.setSingleProperty("id",ch.m_id);

    DatabaseManager::debugQuery(query);
}

void DeviceDao::updateDevice(const Device &dev) const
{
    QMutexLocker locker(&mMutex);

    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < DeviceColumnSize; i++) {
        setStr += (DeviceHeaderList[i] + " = "
                   + (DeviceDataType[i] == "TEXT" ? "'" : "") +
                   dev.getSingleProperty(DeviceHeaderList[i]).toString()
                   + (DeviceDataType[i] == "TEXT" ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE Devices SET " + setStr + " WHERE id = " + dev.getSingleProperty(DeviceHeaderList[0]).toString();

//    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void DeviceDao::removeDevice(int id) const {

    QMutexLocker locker(&mMutex);

    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Devices WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<Device>>> DeviceDao::devices() const
{
    QSqlQuery query("SELECT * FROM Devices", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<Device>>> list(new vector<shared_ptr<Device>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < DeviceColumnSize; i++) {
            properties[DeviceHeaderList[i]] = query.value(DeviceHeaderList[i]);
        }

        if (!properties["Protocol"].toString().isEmpty())
        {
            shared_ptr<Device> dev(new Device(query.value("id").toInt(),properties));
            list->push_back(move(dev));
        }
    }
    return list;
}
