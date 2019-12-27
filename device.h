#ifndef DEVICE_H
#define DEVICE_H

#include <QSerialPort>
#include "devicedata.h"

enum protocol {
  ASCII,
  LRM,
  MODBUS
};

class Device
{
public:
    Device();



private:

    QString serialNum;

    int node_int;

    QString node_str;

    QSerialPort::BaudRate m_baud_rate;

    QSerialPort::Parity m_parity;

    QSerialPort::DataBits m_databits;

    DeviceData m_data;

};

#endif // DEVICE_H
