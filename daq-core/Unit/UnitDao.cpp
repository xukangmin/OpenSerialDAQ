#include "UnitDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Unit.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

UnitDao::UnitDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void UnitDao::init() const
{
    if (!mDatabase.tables().contains("Units")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE Units (";

           qDebug() << "length=" << UnitColumnSize;

           for(int i = 0; i < UnitColumnSize; i++) {
               createQuery += (UnitHeaderList[i] + " " + UnitDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void UnitDao::addUnit(Unit& ch) const
{
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < UnitColumnSize; i++) {
        insertList += (UnitHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < UnitColumnSize; i++) {
        dataList += (":" + UnitHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO Units " + insertList + " VALUES " + dataList);

    for(int i = 1; i < UnitColumnSize; i++) {
        query.bindValue(":" + UnitHeaderList[i], ch.getSingleProperty(UnitHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    DatabaseManager::debugQuery(query);
}

void UnitDao::updateUnit(const Unit &dev) const
{
    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < UnitColumnSize; i++) {
        setStr += (UnitHeaderList[i] + " = "
                   + (UnitDataType[i] == "TEXT" ? "'" : "") +
                   dev.getSingleProperty(UnitHeaderList[i]).toString()
                   + (UnitDataType[i] == "TEXT" ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE Units SET " + setStr + " WHERE id = " + dev.getSingleProperty(UnitHeaderList[0]).toString();

    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void UnitDao::removeUnit(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Units WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<Unit>>> UnitDao::units() const
{
    QSqlQuery query("SELECT * FROM Units", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<Unit>>> list(new vector<shared_ptr<Unit>>());
    while(query.next()) {
        ;

        //        list.append(ch);
        qDebug() << "create new ch=" << query.value("id").toInt();

        QHash<QString, QVariant> properties;

        for(int i = 0; i < UnitColumnSize; i++) {
            properties[UnitHeaderList[i]] = query.value(UnitHeaderList[i]);
        }


        shared_ptr<Unit> dev(new Unit(query.value("id").toInt(),properties));

        list->push_back(move(dev));
    }
    return list;
}
