#include "UnitConversionDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "UnitConversion.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

UnitConversionDao::UnitConversionDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void UnitConversionDao::init() const
{
    if (!mDatabase.tables().contains("UnitConversions")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE UnitConversions (";

           qDebug() << "length=" << UnitConversionColumnSize;

           for(int i = 0; i < UnitConversionColumnSize; i++) {
               createQuery += (UnitConversionHeaderList[i] + " " + UnitConversionDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void UnitConversionDao::addUnitConversion(UnitConversion& ch) const
{
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < UnitConversionColumnSize; i++) {
        insertList += (UnitConversionHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < UnitConversionColumnSize; i++) {
        dataList += (":" + UnitConversionHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO UnitConversions " + insertList + " VALUES " + dataList);

    for(int i = 1; i < UnitConversionColumnSize; i++) {
        query.bindValue(":" + UnitConversionHeaderList[i], ch.getSingleProperty(UnitConversionHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    DatabaseManager::debugQuery(query);
}

void UnitConversionDao::updateUnitConversion(const UnitConversion &dev) const
{
    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < UnitConversionColumnSize; i++) {
        setStr += (UnitConversionHeaderList[i] + " = "
                   + (UnitConversionDataType[i] == "TEXT" ? "'" : "") +
                   dev.getSingleProperty(UnitConversionHeaderList[i]).toString()
                   + (UnitConversionDataType[i] == "TEXT" ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE UnitConversions SET " + setStr + " WHERE id = " + dev.getSingleProperty(UnitConversionHeaderList[0]).toString();

    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void UnitConversionDao::removeUnitConversion(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM UnitConversions WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<UnitConversion>>> UnitConversionDao::unitconversions() const
{
    QSqlQuery query("SELECT * FROM UnitConversions", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<UnitConversion>>> list(new vector<shared_ptr<UnitConversion>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < UnitConversionColumnSize; i++) {
            properties[UnitConversionHeaderList[i]] = query.value(UnitConversionHeaderList[i]);
        }


        shared_ptr<UnitConversion> dev(new UnitConversion(query.value("id").toInt(),properties));

        list->push_back(move(dev));
    }
    return list;
}
