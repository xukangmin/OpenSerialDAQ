#ifndef CHANNEL_H
#define CHANNEL_H

#include <QSerialPort>
#include <QSerialPortInfo>

enum ChannelType{
    Serial,
    IP
};

static const int BAUDRATE[] = {300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200};

enum SerialBaudRate {
    Baud300 = 300,
    Baud600 = 600,
    Baud1200 = 1200,
    Baud4800 = 4800,
    Baud9600 = 9600,
    Baud14400 = 14400,
    Baud19200 = 19200,
    Baud38400 = 38400,
    Baud57600 = 57600,
    Baud115200 = 115200
};

class Channel
{
public:
    Channel(int id,
            QString portName,
            SerialBaudRate baudRate = Baud9600,
            QSerialPort::DataBits dataBits = QSerialPort::Data8,
            QSerialPort::Parity parity = QSerialPort::NoParity,
            QSerialPort::StopBits stopBits = QSerialPort::OneStop);


    Channel(int id,
            QString portName,
            int baudRate = 9600,
            int dataBits = 8,
            QString parity = "None",
            int stopBits = 1);

    int m_id;

    QString m_channelName;

    QString m_portName;

    int m_baudRate;

    QSerialPort::DataBits m_dataBits;

    QSerialPort::Parity m_parity;

    QString m_parityStr;

    QSerialPort::StopBits m_stopBits;


    QString m_baudRateStr;
    QString m_dataBitsStr;
    QString m_stopBitsStr;


    QString m_ipAddress;
    QString m_desAddress;

    int m_ipPort;

    ChannelType m_type;
};

#endif // CHANNEL_H
