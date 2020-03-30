#ifndef DEVICE_H
#define DEVICE_H

#include "daq-core_global.h"
#include <QTimer>
#include "DeviceData.h"
#include <QVariant>
#include "Generic/GenericDefinition.h"
#include <memory>
#include <vector>

class Variable;
class Channel;

struct Parameter {
    QString name;
    int query_value;
    int value_int;
    QString value_string;
    QString description;

};

struct ParseFormat {
    QString type;
    QString name;
    int location;
    QString usage;
    QString unit;
    QString physics;
};

struct Command {
    int cmd_id;
    QString name;
    QVector<QString> query_cmd_arr;
    QByteArray query_cmd;
    QString parse_method;
    QString description;
    uint8_t header;
    uint8_t cmd;
    uint8_t api;
    uint8_t checksum;
    QString check_sum_rule;
    uint8_t length;
    bool isDAQ;
    int interval;
    QVector<ParseFormat> parse_info;
};

static const QString DeviceHeaderList[] = {"id","Name","NodeID","Protocol","ChannelID"};
static const QString DeviceDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT","INTEGER", "TEXT", "INTEGER"};
static const int DeviceColumnSize = 5;

class DAQCORESHARED_EXPORT Device : public QObject, public GenericDefinition
{

Q_OBJECT

public:

    Device(int id, QHash<QString, QVariant> properties);

    //QVector<Variable*> *m_var_list;

    std::vector<std::shared_ptr<Variable>> mVariableList;

    std::shared_ptr<Channel> mChannel;

    QVector<QTimer*> mTimerPool;

    QVector<Command> mCommands;

    std::vector<QHash<QString, QVariant>> mVariablePropertiesList;

    void loadFromConfig(QString protocol_name);

    QByteArray buildQueryCmd(Command cmd);
    QByteArray buildQueryCmd(QString cmdName);
    void setChannel(const std::shared_ptr<Channel>& channel);
    void clearChannel();

    void startDAQ(int simulation = 0);
    void endDAQ(int simulation = 0);


    std::vector<QHash<QString,QVariant>> parseRxData(QByteArray rx_data, int cmd_id);

    DeviceData m_deviceData;

    QTimer* mSimulationTimer;

};

#endif // DEVICE_H
