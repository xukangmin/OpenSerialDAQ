#ifndef DEVICE_H
#define DEVICE_H

#include <QSerialPort>
#include <QTimer>
#include "Variable/Variable.h"
#include "DeviceData.h"

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

class Device : public QObject
{

Q_OBJECT

public:
    Device(int node_id);

    Device(int node_id, QString protocol_name);

    Device(int id, int node_id, QString protocol_name, QVector<Variable*> *var_list);

    Device(int id, QString name, int node_id, QString protocol_name, QVector<Variable*> *var_list);


    int m_device_id;

    QString m_name;

    int m_node_id;



    QString m_protocol;

    QVector<Variable*> *m_var_list;

    QString m_serial_number;


    QVector<QTimer*> m_timer_pool;


    QVector<Command> m_commands;

    void setID(int id);
    void loadFromConfig(QString protocol_name);

    QByteArray buildQueryCmd(Command cmd);
    QByteArray buildQueryCmd(QString cmdName);

    QVector<DeviceData> parseRxData(QByteArray rx_data, int cmd_id);

    DeviceData m_deviceData;

};

#endif // DEVICE_H
