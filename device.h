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

class Device
{
public:
    Device();

    QString serial_number;

    int node_int;

    QString node_str;

    DeviceData m_daq_data;

    QByteArray m_daq_cmd;

    QByteArray m_zero_cmd;

    QVector<QByteArray> m_parameter_cmd;

    QVector<Parameter> m_parameters;

    void loadFromConfig(QString protocol_name, QString device_name, int node_id);

};

#endif // DEVICE_H
