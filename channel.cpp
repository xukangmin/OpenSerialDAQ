#include "channel.h"

Channel::Channel(QString portName,
        SerialBaudRate baudRate,
        QSerialPort::DataBits dataBits,
        QSerialPort::Parity parity,
         QSerialPort::StopBits stopBits) :
     m_portName(portName),
     m_baudRate(baudRate),
     m_dataBits(dataBits),
     m_parity(parity),
     m_stopBits(stopBits)
{
    m_type = Serial;
}


Channel::Channel(QString portName,
                 int baudRate,
                 int dataBits,
                 QString parity,
                 int stopBits):
                 m_portName(portName)
{

}
