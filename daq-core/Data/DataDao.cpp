#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Data.h"
#include "DataDao.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

DataDao::DataDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void DataDao::init() const
{
    if (!mDatabase.tables().contains("Datas")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE Datas (";

           qDebug() << "length=" << DataColumnSize;

           for(int i = 0; i < DataColumnSize; i++) {
               createQuery += (DataHeaderList[i] + " " + DataDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void DataDao::addData(Data& ch) const
{
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < DataColumnSize; i++) {
        insertList += (DataHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < DataColumnSize; i++) {
        dataList += (":" + DataHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO Datas " + insertList + " VALUES " + dataList);

    for(int i = 1; i < DataColumnSize; i++) {
        query.bindValue(":" + DataHeaderList[i], ch.getSingleProperty(DataHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    ch.setSingleProperty("id",ch.m_id);

    DatabaseManager::debugQuery(query);
}

void DataDao::updateData(const Data &data) const
{
    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < DataColumnSize; i++) {
        setStr += (DataHeaderList[i] + " = "
                   + (DataDataType[i] == "TEXT" ? "'" : "") +
                   data.getSingleProperty(DataHeaderList[i]).toString()
                   + (DataDataType[i] == "TEXT" ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE Datas SET " + setStr + " WHERE id = " + data.getSingleProperty(DataHeaderList[0]).toString();

//    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void DataDao::removeData(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Datas WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<Data>>> DataDao::datas() const
{
    QSqlQuery query("SELECT * FROM Datas", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<Data>>> list(new vector<shared_ptr<Data>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < DataColumnSize; i++) {
            properties[DataHeaderList[i]] = query.value(DataHeaderList[i]);
        }


        shared_ptr<Data> dev(new Data(query.value("id").toInt(),properties));

        list->push_back(move(dev));
    }
    return list;
}
