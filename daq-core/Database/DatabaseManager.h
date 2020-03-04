#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "Channel/Channel.h"
#include "Channel/ChannelDao.h"
#include "Device/DeviceDao.h"
#include "Variable/VariableDao.h"
#include "Unit/UnitDao.h"
#include "Data/DataDao.h"

class QSqlDatabase;
class QSqlQuery;

const QString DATABASE_FILENAME = "SerialDAQData.db";

class DatabaseManager
{
public:
     static DatabaseManager& instance();
     ~DatabaseManager();

    static void debugQuery(const QSqlQuery& query);

//    void initDevice();

//    void initData();

//    void initVariable();

//    void initChannel();

//    void init();

//    void insertData(DeviceData da, int devid);


//    int insertDevice(QString name, int node, QString protocol, int ch_id = 0);
//    int insertVariable(QString name, QString unit, int deviceId = 0, QString equation = "N/A");
//    int insertChannel(Channel ch);

//    void queryData();
//    void removeChannel(int id) const;
//    void removeDevice(int id) const;

//    bool getDevice(int id, QVector<Device*> *dev_list, QVector<Variable*> *var_list = nullptr);
//    bool getAllDevice(QVector<Device*> *dev_list, QVector<Variable*> *var_list = nullptr);
//    bool getUnassignedDevice(QVector<Device*> *dev_list, QVector<Variable*> *var_list = nullptr);
//     bool getDeviceWithChannelID(int channelID, QVector<Device*> *dev_list, QVector<Variable*> *var_list = nullptr);
//    QVector<Channel> getAllChannel();
//    void updateDeviceWithChannelID(int deviceID, int channelID);

//    void resetDeviceBinding(int devID);

//    bool checkVariableExist(QString name, int deviceID);

protected:
    DatabaseManager(const QString& path = DATABASE_FILENAME);

private:
    std::unique_ptr<QSqlDatabase> mDatabase;

public:
    const ChannelDao channelDao;
    const DeviceDao deviceDao;
    const VariableDao variableDao;
    const UnitDao unitDao;
    const DataDao dataDao;
};

#endif // DATABASEMANAGER_H
