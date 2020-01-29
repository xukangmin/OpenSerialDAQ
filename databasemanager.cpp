#include "databasemanager.h"
#include <QDebug>

#include <QSqlError>

DatabaseManager&DatabaseManager::instance()
{
    static DatabaseManager singleton;
    return singleton;
}

DatabaseManager::DatabaseManager(const QString& path) :
    m_database(QSqlDatabase::addDatabase("QSQLITE"))
{
    m_database.setDatabaseName(path);

    bool openStatus = false;
    openStatus = m_database.open();
    qDebug() << "Database connection: " << (openStatus ? "OK" : "Error");
}

DatabaseManager::~DatabaseManager() {
    m_database.close();
}

void DatabaseManager::insertData(DeviceData da, int devid) {
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO Data (DeviceID, DataTypeID, TimeStamp, Value) VALUES (:DeviceID, :DataTypeID, :TimeStamp, :Value)");
    query.bindValue(":DeviceID", devid);
    query.bindValue(":DataTypeID", da.m_dataIndex);
    query.bindValue(":TimeStamp", da.m_currentTimeStamp);
    query.bindValue(":Value", da.m_currentData);
    query.exec();
    debugQuery(query);

}

void DatabaseManager::initDevice() {

    if (!m_database.tables().contains("Devices")) {
        QSqlQuery query(m_database);
        query.exec("CREATE TABLE Devices ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "Node INTEGER, "
                   "Protocol TEXT )");
        debugQuery(query);
    }

}

void DatabaseManager::initDataType() {

    if (!m_database.tables().contains("DataType")) {
        QSqlQuery query(m_database);
        query.exec("CREATE TABLE DataType ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "DeviceID INTEGER"
                   "Name TEXT, "
                   "Unit TEXT)");
        debugQuery(query);
    }
}

void DatabaseManager::initData() {
    if (!m_database.tables().contains("Data")) {
        QSqlQuery query(m_database);
        query.exec("CREATE TABLE Data ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "DeviceID INTEGER, "
                   "DataTypeID INTEGER,"
                   "TimeStamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                   "Value DOUBLE)");

        debugQuery(query);
    }
}

void DatabaseManager::init() {
    initDevice();
    initData();
    initDataType();
}

int DatabaseManager::insertDevice(int node, QString protocol) {
    QSqlQuery query(m_database);

    query.prepare("INSERT INTO Devices (Node,Protocol) VALUES (:Node,:Protocol)");
    query.bindValue(":Node",node);
    query.bindValue(":Protocol", protocol);
    query.exec();
    debugQuery(query);
    return query.lastInsertId().toInt();

}

void DatabaseManager::removeDevice(int id) const
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM Devices WHERE id = (:id)");
    query.bindValue(":id", id);
    query.exec();
    debugQuery(query);
}

QVector<Device*> DatabaseManager::getAllDevice() {
    QSqlQuery query("SELECT * FROM Devices", m_database);

    query.exec();

    QVector<Device*> list;

    while(query.next()) {
        Device* dev = new Device(query.value("id").toInt(), query.value("Node").toInt(), query.value("Protocol").toString());
        list.append(dev);
    }

    return list;

}


void DatabaseManager::debugQuery(const QSqlQuery& query) const
{
    if (query.lastError().type() == QSqlError::ErrorType::NoError) {
        qDebug() << "Query OK:"  << query.lastQuery();
    } else {
       qWarning() << "Query KO:" << query.lastError().text();
       qWarning() << "Query text:" << query.lastQuery();
    }
}
