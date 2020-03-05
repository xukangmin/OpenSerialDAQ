#include "UnitTypeDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "UnitType.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

UnitTypeDao::UnitTypeDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void UnitTypeDao::init() const
{
    if (!mDatabase.tables().contains("UnitTypes")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE UnitTypes (";

           qDebug() << "length=" << UnitTypeColumnSize;

           for(int i = 0; i < UnitTypeColumnSize; i++) {
               createQuery += (UnitTypeHeaderList[i] + " " + UnitTypeDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void UnitTypeDao::addUnitType(UnitType& ch) const
{
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < UnitTypeColumnSize; i++) {
        insertList += (UnitTypeHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < UnitTypeColumnSize; i++) {
        dataList += (":" + UnitTypeHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO UnitTypes " + insertList + " VALUES " + dataList);

    for(int i = 1; i < UnitTypeColumnSize; i++) {
        query.bindValue(":" + UnitTypeHeaderList[i], ch.getSingleProperty(UnitTypeHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    DatabaseManager::debugQuery(query);
}

void UnitTypeDao::updateUnitType(const UnitType &dev) const
{
    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < UnitTypeColumnSize; i++) {
        setStr += (UnitTypeHeaderList[i] + " = "
                   + (UnitTypeDataType[i] == "TEXT" ? "'" : "") +
                   dev.getSingleProperty(UnitTypeHeaderList[i]).toString()
                   + (UnitTypeDataType[i] == "TEXT" ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE UnitTypes SET " + setStr + " WHERE id = " + dev.getSingleProperty(UnitTypeHeaderList[0]).toString();

    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void UnitTypeDao::removeUnitType(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM UnitTypes WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<UnitType>>> UnitTypeDao::unittypes() const
{
    QSqlQuery query("SELECT * FROM UnitTypes", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<UnitType>>> list(new vector<shared_ptr<UnitType>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < UnitTypeColumnSize; i++) {
            properties[UnitTypeHeaderList[i]] = query.value(UnitTypeHeaderList[i]);
        }


        shared_ptr<UnitType> dev(new UnitType(query.value("id").toInt(),properties));

        list->push_back(move(dev));
    }
    return list;
}
