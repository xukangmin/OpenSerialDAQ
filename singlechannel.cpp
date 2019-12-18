#include "singlechannel.h"
#include <QSerialPort>
#include <QDebug>

SingleChannel::SingleChannel(QObject *parent) :
    QThread(parent)
{
    m_timer.setInterval(1000);
}


SingleChannel::~SingleChannel()
{

}


void SingleChannel::startChannel() {
    if (!isRunning()){
        start();
    }
}


void SingleChannel::stopChannel() {

}


void SingleChannel::run()
{


    while(1) {
        QSerialPort serial;

        serial.setPortName("COM5");
        serial.setBaudRate(QSerialPort::Baud9600);

        if (!serial.open(QIODevice::ReadWrite)) {
            return;
        }

        QString req = "A\r";
        // send request

        const QByteArray reqData = req.toUtf8();
        serial.write(reqData);
        if (serial.waitForBytesWritten(1000)) {
            qDebug() << "data sent";
        }


        if (serial.waitForReadyRead(1000)) {
            QByteArray requestData = serial.readAll();
            while (serial.waitForReadyRead(10))
                requestData += serial.readAll();


        }
    }
}
