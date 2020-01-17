#ifndef DEVICE_H
#define DEVICE_H

#include <QSerialPort>
#include "devicedata.h"

enum protocol {
  ASCII,
  LRM,
  MODBUS
};

struct Parameter {
    QString name;
    int query_value;
    int value_int;
    QString value_string;
    QString description;

};

struct DataFormat {
    QString type;
    QString name;
    int location;
    QString usage;
    QString unit;
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
    QVector<DataFormat> data_formats;
};

class Device
{
public:
    Device(int node_id);

    Device(int node_id, QString protocol_name);

    QString m_serial_number;

    int m_node_id;

    int m_device_id;

    QVector<Command> m_commands;

    void loadFromConfig(QString protocol_name);

    QByteArray buildQueryCmd(QString cmdName);

    void parseRxData(QByteArray rx_data, int cmd_id);

};

#endif // DEVICE_H
