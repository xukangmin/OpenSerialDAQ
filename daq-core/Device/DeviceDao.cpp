#include "DeviceDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Device.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

DeviceDao::DeviceDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void DeviceDao::init() const
{
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
        query.bindValue(":" + DeviceHeaderList[i], ch.getProperty(DeviceHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    DatabaseManager::debugQuery(query);
}

void DeviceDao::updateDevice(const Device &ch) const
{

}

void DeviceDao::removeDevice(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Devices WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Device>>> DeviceDao::devices() const
{
    QSqlQuery query("SELECT * FROM Devices", mDatabase);
    query.exec();
    unique_ptr<vector<unique_ptr<Device>>> list(new vector<unique_ptr<Device>>());
    while(query.next()) {
        ;

        //        list.append(ch);
        qDebug() << "create new ch=" << query.value("id").toInt();

        QHash<QString, QVariant> properties;

        for(int i = 0; i < DeviceColumnSize; i++) {
            properties[DeviceHeaderList[i]] = query.value(DeviceHeaderList[i]);
        }


        unique_ptr<Device> ch(new Device(query.value("id").toInt(),properties));

        list->push_back(move(ch));
    }
    return list;
}
