#include "VariableGroupDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "VariableGroup.h"
#include "Database/DatabaseManager.h"
#include <QDebug>

using namespace std;

VariableGroupDao::VariableGroupDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void VariableGroupDao::init() const
{
    if (!mDatabase.tables().contains("VariableGroups")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE VariableGroups (";

           qDebug() << "length=" << VariableGroupColumnSize;

           for(int i = 0; i < VariableGroupColumnSize; i++) {
               createQuery += (VariableGroupHeaderList[i] + " " + VariableGroupDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void VariableGroupDao::addVariableGroup(VariableGroup& ch) const
{
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < VariableGroupColumnSize; i++) {
        insertList += (VariableGroupHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < VariableGroupColumnSize; i++) {
        dataList += (":" + VariableGroupHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO VariableGroups " + insertList + " VALUES " + dataList);

    for(int i = 1; i < VariableGroupColumnSize; i++) {
        query.bindValue(":" + VariableGroupHeaderList[i], ch.getSingleProperty(VariableGroupHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    ch.setSingleProperty("id",ch.m_id);

    DatabaseManager::debugQuery(query);
}

void VariableGroupDao::updateVariableGroup(const VariableGroup &dev) const
{
    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < VariableGroupColumnSize; i++) {
        setStr += (VariableGroupHeaderList[i] + " = "
                   + (VariableGroupDataType[i] == "TEXT" ? "'" : "") +
                   dev.getSingleProperty(VariableGroupHeaderList[i]).toString()
                   + (VariableGroupDataType[i] == "TEXT" ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE VariableGroups SET " + setStr + " WHERE id = " + dev.getSingleProperty(VariableGroupHeaderList[0]).toString();

//    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void VariableGroupDao::removeVariableGroup(int id) const {
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM VariableGroups WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<VariableGroup>>> VariableGroupDao::variablegroups() const
{
    QSqlQuery query("SELECT * FROM VariableGroups", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<VariableGroup>>> list(new vector<shared_ptr<VariableGroup>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < VariableGroupColumnSize; i++) {
            properties[VariableGroupHeaderList[i]] = query.value(VariableGroupHeaderList[i]);
        }


        shared_ptr<VariableGroup> dev(new VariableGroup(query.value("id").toInt(),properties));

        list->push_back(move(dev));
    }
    return list;
}
