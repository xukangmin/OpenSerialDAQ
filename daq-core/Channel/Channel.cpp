#include "Channel.h"
#include "Thread/ThreadChannel.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

Channel::Channel(int id,
                 QString portName,
                 int baudRate,
                 int dataBits,
                 QString parity,
                 int stopBits):
                 m_id(id),
                 m_portName(portName),
                 m_baudRate(baudRate),
                 m_baudRateStr(QString::number(baudRate)),
                 m_dataBitsStr(QString::number(dataBits)),
                 m_stopBitsStr(QString::number(stopBits)),
                 m_threadChannel(new ThreadChannel(*this))
{
    qDebug() << "channel " << m_id << " created";
    m_type = Serial;

    switch (dataBits) {
        case 8:
            m_dataBits = QSerialPort::Data8;
            break;
        case 7:
            m_dataBits = QSerialPort::Data7;
            break;
        case 6:
            m_dataBits = QSerialPort::Data6;
            break;
        case 5:
            m_dataBits = QSerialPort::Data5;
            break;
        default:
            m_dataBits = QSerialPort::Data8;
            break;
    }

    if (parity == "None" or parity == "NONE" or parity == "N/A" or parity == "No" or parity == "none") {
        m_parity = QSerialPort::NoParity;
        m_parityStr = "None";
    } else if (parity == "ODD" or parity == "Odd" or parity == "odd") {
        m_parity = QSerialPort::OddParity;
        m_parityStr = "Odd";
    } else if (parity == "EVEN" or parity == "Even" or parity == "even") {
        m_parity = QSerialPort::EvenParity;
        m_parityStr = "Even";
    } else {
        m_parity = QSerialPort::NoParity;
        m_parityStr = "None";
    }

    switch (stopBits) {
        case 1:
            m_stopBits = QSerialPort::OneStop;
            break;
        case 2:
            m_stopBits = QSerialPort::TwoStop;
            break;
        default:
            m_stopBits = QSerialPort::OneStop;
            break;
    }


}

Channel::~Channel() {
    qDebug() << "channel " << m_id << "destroyed";
   // m_threadChannel->stopDAQ();
    //m_threadChannel->deleteLater();

}

void Channel::startChannel()
{
    m_threadChannel->startChannel();
}

void Channel::stopChannel()
{
    m_threadChannel->stopChannel();
}
void Channel::addDeviceToChannel(int device_id)
{
    //m_threadChannel->addDevice()
}
