#include "singlechannel.h"
#include <QSerialPort>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SingleChannel::SingleChannel(QObject *parent) :
    QThread(parent)
{
}


SingleChannel::~SingleChannel()
{

}

void SingleChannel::addDevice(Device* dev) {
    m_device_pool.append(dev);

    // setup DAQ timer
    QTimer *m_timer = new QTimer(this);
    m_timer->setInterval(80);
    connect(m_timer, &QTimer::timeout, this, [=]() {
        Packet *pac = new Packet();
        pac->m_query_bytes = dev->buildQueryCmd("ReadData");
        pac->m_packet_id = rand();
        pac->device_id = dev->m_device_id;
        m_packet_queue.enqueue(pac);
    });
    m_timer->start();
    m_timer_pool.append(m_timer);
}


void SingleChannel::startChannel() {
    if (!isRunning()){
        start();
    }
}


void SingleChannel::stopChannel() {
    if (isRunning()) {
        quit();
    }
}


void SingleChannel::run()
{
    QSerialPort serial;

    serial.setPortName("COM49");
    serial.setBaudRate(QSerialPort::Baud115200);

    if (!serial.open(QIODevice::ReadWrite)) {
        //Todo: emit a signal here

        return;
    }

    while(1) {
        if (!m_packet_queue.isEmpty()) {
            Packet* pac = m_packet_queue.dequeue();

            serial.write(pac->m_query_bytes);
            if (serial.waitForBytesWritten(1000)) {
                qDebug() << "data sent";
            }


            if (serial.waitForReadyRead(1000)) {
                QByteArray requestData = serial.readAll();
                while (serial.waitForReadyRead(10))
                    requestData += serial.readAll();

                qDebug() << "received";
                qDebug() << requestData;

                pac->m_response_bytes = requestData;

                m_device_pool[0]->parseRxData(pac->m_response_bytes, 0);

                // use queue to process data find related device, parse data and save data to db

                delete pac;
            } else {
                qDebug() << "time out, no data received";
            }
        }
        msleep(1);
    }
}
