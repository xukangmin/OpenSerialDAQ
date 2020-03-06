#include "DatabaseManager.h"
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

DatabaseManager&DatabaseManager::instance()
{
    static DatabaseManager singleton;
    return singleton;
}

DatabaseManager::DatabaseManager(const QString& path) :
    mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
    channelDao(*mDatabase),
    deviceDao(*mDatabase),
    variableDao(*mDatabase),
    dataDao(*mDatabase),
    variableGroupDao(*mDatabase)
{
    mDatabase->setDatabaseName(path);

    bool openStatus = mDatabase->open();
    qDebug() << "Database connection: " << (openStatus ? "OK" : "Error");

    channelDao.init();
    deviceDao.init();
    variableDao.init();
    dataDao.init();
    variableGroupDao.init();

}


//DatabaseManager::DatabaseManager(const QString& path) :
//    m_database(QSqlDatabase::addDatabase("QSQLITE"))
//{
//    m_database.setDatabaseName(path);

//    bool openStatus = false;
//    openStatus = m_database.open();
//    qDebug() << "Database connection: " << (openStatus ? "OK" : "Error");
//}

DatabaseManager::~DatabaseManager() {
    mDatabase->close();
}

//void DatabaseManager::insertData(DeviceData da, int devid) {
//    QSqlQuery query(m_database);
//    query.prepare("INSERT INTO Data (DeviceID, DataTypeID, TimeStamp, Value) VALUES (:DeviceID, :DataTypeID, :TimeStamp, :Value)");
//    query.bindValue(":DeviceID", devid);
//    query.bindValue(":DataTypeID", da.m_dataIndex);
//    query.bindValue(":TimeStamp", da.m_currentTimeStamp);
//    query.bindValue(":Value", da.m_currentData);
//    query.exec();
//    debugQuery(query);

//}

//void DatabaseManager::initDevice() {

//    if (!m_database.tables().contains("Devices")) {
//        QSqlQuery query(m_database);
//        query.exec("CREATE TABLE Devices ("
//                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                   "Name TEXT,"
//                   "Node INTEGER, "
//                   "Protocol TEXT,"
//                   "ChannelID INTEGER)");
//        debugQuery(query);
//    }

//}

//void DatabaseManager::initChannel() {

//    if (!m_database.tables().contains("Channels")) {
//        QSqlQuery query(m_database);
//        query.exec("CREATE TABLE Channels ("
//                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                   "Name TEXT, "
//                   "Type TEXT, "
//                   "ComPort TEXT,"
//                   "BaudRate INTEGER,"
//                   "Parity TEXT,"
//                   "DataBits INTEGER,"
//                   "StopBits INTEGER,"
//                   "Source TEXT,"
//                   "Port INTEGER"
//                   ")");
//        debugQuery(query);
//    }

//}

//void DatabaseManager::initVariable() {

//    if (!m_database.tables().contains("Variable")) {
//        QSqlQuery query(m_database);
//        query.exec("CREATE TABLE Variable ("
//                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                   "Name TEXT, "
//                   "Unit TEXT, "
//                   "DeviceID INTEGER, "
//                   "Equation TEXT,"
//                   "GourpID INTEGER"
//                   ")");
//        debugQuery(query);
//    }
//}

//void DatabaseManager::initData() {
//    if (!m_database.tables().contains("Data")) {
//        QSqlQuery query(m_database);
//        query.exec("CREATE TABLE Data ("
//                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                   "DeviceID INTEGER, "
//                   "DataTypeID INTEGER,"
//                   "TimeStamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
//                   "Value DOUBLE)");

//        debugQuery(query);
//    }
//}

//void DatabaseManager::init() {
//    initDevice();
//    initData();
//    initVariable();
//    initChannel();
//}

//int DatabaseManager::insertDevice(QString name, int node, QString protocol, int ch_id) {
//    QSqlQuery query(m_database);

//    query.prepare("INSERT INTO Devices (Name,Node,Protocol,ChannelID) VALUES (:Name,:Node,:Protocol,:ChannelID)");
//    query.bindValue(":Name",name);
//    query.bindValue(":Node",node);
//    query.bindValue(":Protocol", protocol);
//    query.bindValue(":ChannelID", ch_id);
//    query.exec();
//    debugQuery(query);
//    return query.lastInsertId().toInt();

//}

//int DatabaseManager::insertVariable(QString name, QString unit, int deviceId, QString equation) {
//    QSqlQuery query(m_database);

//    query.prepare("INSERT INTO Variable (Name,Unit,DeviceID,Equation) VALUES (:name,:unit,:deviceId,:equation)");
//    query.bindValue(":name",name);
//    query.bindValue(":unit",unit);
//    query.bindValue(":deviceId", deviceId);
//    query.bindValue(":equation", equation);
//    query.exec();
//    debugQuery(query);
//    return query.lastInsertId().toInt();

//}


//int DatabaseManager::insertChannel(Channel ch) {
//    QSqlQuery query(m_database);

//    query.prepare("INSERT INTO Channels (Type,ComPort,BaudRate,Parity,DataBits,StopBits) VALUES (:Type,:ComPort,:BaudRate,:Parity,:DataBits,:StopBits)");

//    query.bindValue(":Type",(ch.m_type == Serial ? "Serial" : "IP"));
//    query.bindValue(":ComPort", ch.m_portName);
//    query.bindValue(":BaudRate", ch.m_baudRate);
//    query.bindValue(":Parity", ch.m_parityStr);
//    query.bindValue(":DataBits", ch.m_dataBits);
//    query.bindValue(":StopBits", ch.m_stopBits);
//    query.exec();
//    debugQuery(query);
//    return query.lastInsertId().toInt();

//}

//void DatabaseManager::removeChannel(int id) const
//{
//    QSqlQuery query(m_database);
//    query.prepare("DELETE FROM Channels WHERE id = (:id)");
//    query.bindValue(":id", id);
//    query.exec();
//    debugQuery(query);
//}


//void DatabaseManager::removeDevice(int id) const
//{
//    QSqlQuery query(m_database);
//    query.prepare("DELETE FROM Devices WHERE id = (:id)");
//    query.bindValue(":id", id);
//    query.exec();
//    debugQuery(query);
//}

//bool DatabaseManager::getDevice(int id, QVector<Device*> *dev_list, QVector<Variable*> *var_list) {
//    QSqlQuery query(m_database);

//    query.prepare("SELECT * FROM Devices WHERE id=:deviceID");

//    query.bindValue(":deviceID", id);

//    query.exec();

////    while(query.next()) {
////        dev_list->append(Device(query.value("id").toInt(), query.value("Node").toInt(), query.value("Protocol").toString(), var_list));
////    }

//    return true;
//}

//bool DatabaseManager::getAllDevice(QVector<Device*> *dev_list, QVector<Variable*> *var_list) {
//    QSqlQuery query("SELECT * FROM Devices", m_database);

//    query.exec();

//    while(query.next()) {
//        Device *dev = new Device(query.value("id").toInt(), query.value("Node").toInt(), query.value("Protocol").toString(), var_list);
//        dev_list->append(dev);
//    }

//    return true;
//}

//void DatabaseManager::updateDeviceWithChannelID(int deviceID, int channelID) {
//    QSqlQuery query(m_database);

//    query.prepare("UPDATE Devices SET ChannelID=:channelID WHERE id=:deviceID");

//    query.bindValue(":channelID", channelID);

//    query.bindValue(":deviceID", deviceID);

//    query.exec();

//    debugQuery(query);
//}

//bool DatabaseManager::getDeviceWithChannelID(int channelID, QVector<Device*> *dev_list, QVector<Variable*> *var_list) {
//    QSqlQuery query(m_database);

//    query.prepare("SELECT * FROM Devices WHERE ChannelID = :channelID");

//    query.bindValue(":channelID", channelID);

//    query.exec();

//    QVector<Device> list;

////    while(query.next()) {
////        dev_list->append(Device(query.value("id").toInt(), query.value("Node").toInt(), query.value("Protocol").toString(), var_list));
////    }

//    return true;

//}

//void DatabaseManager::resetDeviceBinding(int devID) {
//    QSqlQuery query(m_database);

//    query.prepare("UPDATE Devices SET ChannelID = 0 WHERE id=:devID");

//    query.bindValue(":devID", devID);

//    query.exec();
//}

//bool DatabaseManager::getUnassignedDevice(QVector<Device*> *dev_list, QVector<Variable*> *var_list) {
//    QSqlQuery query("SELECT * FROM Devices WHERE ChannelID = 0", m_database);

//    query.exec();

//    QVector<Device> list;

////    while(query.next()) {
////        dev_list->append(Device(query.value("id").toInt(), query.value("Node").toInt(), query.value("Protocol").toString(), var_list));
////    }

//    return true;

//}

//bool DatabaseManager::checkVariableExist(QString name, int deviceID) {
//    QSqlQuery query(m_database);

//    query.prepare("SELECT COUNT(*) FROM Variable WHERE Name=:name AND DeviceID=:deviceID");
//    query.bindValue(":name", name);
//    query.bindValue(":deviceID",deviceID);
//    query.exec();

//    int cont = 0;

//    if(query.next()) {
//        cont = query.value(0).toInt();
//    }

//    if (cont == 0) {
//        return false;
//    } else {
//        return true;
//    }

//}

//QVector<Channel> DatabaseManager::getAllChannel() {
//    QSqlQuery query("SELECT * FROM Channels", m_database);

//    query.exec();

//    QVector<Channel> list;

//    while(query.next()) {
//        Channel ch = Channel(query.value("id").toInt(),
//                             query.value("ComPort").toString(),
//                              query.value("BaudRate").toInt(),
//                              query.value("DataBits").toInt(),
//                              query.value("Parity").toString(),
//                              query.value("StopBits").toInt());

//        list.append(ch);
//    }

//    return list;

//}


void DatabaseManager::debugQuery(const QSqlQuery& query)
{
    if (query.lastError().type() == QSqlError::ErrorType::NoError) {
        qDebug() << "Query OK:"  << query.lastQuery();
    } else {
       qWarning() << "Query KO:" << query.lastError().text();
       qWarning() << "Query text:" << query.lastQuery();
    }
}
