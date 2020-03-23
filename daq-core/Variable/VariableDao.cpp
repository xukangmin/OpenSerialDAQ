#include "VariableDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Variable.h"
#include "Database/DatabaseManager.h"
#include <QDebug>
#include <QMutex>

using namespace std;

VariableDao::VariableDao(QSqlDatabase& database, QMutex& dbMutex):
    mDatabase(database), mMutex(dbMutex)
{
}

void VariableDao::init() const
{
    QMutexLocker locker(&mMutex);
    if (!mDatabase.tables().contains("Variables")) {
            QSqlQuery query(mDatabase);

           QString createQuery = "CREATE TABLE Variables (";

           qDebug() << "length=" << VariableColumnSize;

           for(int i = 0; i < VariableColumnSize; i++) {
               createQuery += (VariableHeaderList[i] + " " + VariableDataType[i] + ",");
           }

           createQuery = createQuery.left(createQuery.length() - 1);

           createQuery += ")";

            query.exec(createQuery);

            DatabaseManager::debugQuery(query);
        }
}

void VariableDao::addVariable(Variable& ch) const
{
    QMutexLocker locker(&mMutex);
    QSqlQuery query(mDatabase);

    QString insertList = "(";

    for(int i = 1; i < VariableColumnSize; i++) {
        insertList += (VariableHeaderList[i] + ",");
    }

    insertList = insertList.left(insertList.length() - 1);

    insertList += ")";

    QString dataList = "(";

    for(int i = 1; i < VariableColumnSize; i++) {
        dataList += (":" + VariableHeaderList[i] + ",");
    }

    dataList = dataList.left(dataList.length() - 1);

    dataList += ")";

    query.prepare("INSERT INTO Variables " + insertList + " VALUES " + dataList);

    for(int i = 1; i < VariableColumnSize; i++) {
        query.bindValue(":" + VariableHeaderList[i], ch.getSingleProperty(VariableHeaderList[i]));
    }

    query.exec();

    ch.m_id = query.lastInsertId().toInt();

    ch.setSingleProperty("id",ch.m_id);

    DatabaseManager::debugQuery(query);
}

void VariableDao::updateVariable(const Variable &dev) const
{
    QMutexLocker locker(&mMutex);
    QSqlQuery query(mDatabase);


    QString setStr = "";

    for(int i = 1; i < VariableColumnSize; i++) {


        setStr += VariableHeaderList[i] + " = ";
        setStr += dev.getSingleProperty(VariableHeaderList[i]).isNull() ? "NULL," :
                    (((VariableDataType[i] == "TEXT" || VariableDataType[i] == "DATETIME") ? "'" : "") +
                   dev.getSingleProperty(VariableHeaderList[i]).toString()
                   + ((VariableDataType[i] == "TEXT" || VariableDataType[i] == "DATETIME") ? "'" : "") + ",");
    }

    setStr = setStr.left(setStr.length() - 1);

    QString queryStr = "UPDATE Variables SET " + setStr + " WHERE id = " + dev.getSingleProperty(VariableHeaderList[0]).toString();

//    qDebug() << queryStr;

    query.prepare(queryStr);

    query.exec();

    DatabaseManager::debugQuery(query);
}

void VariableDao::removeVariable(int id) const {
    QMutexLocker locker(&mMutex);
    QSqlQuery query(mDatabase);
    query.prepare("DELETE FROM Variables WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<shared_ptr<Variable>>> VariableDao::variables() const
{
    QSqlQuery query("SELECT * FROM Variables", mDatabase);
    query.exec();
    unique_ptr<vector<shared_ptr<Variable>>> list(new vector<shared_ptr<Variable>>());
    while(query.next()) {

        QHash<QString, QVariant> properties;

        for(int i = 0; i < VariableColumnSize; i++) {
            properties[VariableHeaderList[i]] = query.value(VariableHeaderList[i]);
        }


        shared_ptr<Variable> dev(new Variable(query.value("id").toInt(),properties));

        list->push_back(move(dev));
    }
    return list;
}
