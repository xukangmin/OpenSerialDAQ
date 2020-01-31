#include "singlechannel.h"
#include <QSerialPort>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThreadPool>
#include "mainwindow.h"
#include "databasemanager.h"

SingleChannel::SingleChannel(Channel ch, QObject *parent) :
     QThread(parent),
     m_ch(ch),
     m_stop(false),
     m_ch_id(ch.m_id)
{
}


SingleChannel::~SingleChannel()
{
    qDebug() << "channel " << m_ch_id << " destroyed";
    //DatabaseManager::instance().removeChannel(m_ch_id);
    m_stop = true;


}

void SingleChannel::addDevice(Device* dev) {
    m_device_pool.append(dev);

    foreach(Command cmd, dev->m_commands) {
        if (cmd.isDAQ) {
            QTimer *m_timer = new QTimer(this);
            m_timer->setInterval(cmd.interval);
            connect(m_timer, &QTimer::timeout,this,[=]() {
                Packet *pac = new Packet();
                pac->m_packet_id = rand();
                pac->m_query_bytes = dev->buildQueryCmd(cmd);
                pac->dev = dev;
                pac->m_cmd_id = cmd.cmd_id;
                m_packet_queue.enqueue(pac);
            });
            m_timer_pool.append(m_timer);
        }
    }
}


void SingleChannel::startChannel() {
    if (!isRunning()){
        m_stop = false;
        start();
        startDAQ();
    }
}


void SingleChannel::stopChannel() {
    if (isRunning()) {
        m_stop = true;
        quit();
    }
}

void SingleChannel::startDAQ() {
    foreach(QTimer *tm, m_timer_pool) {
        tm->start();
    }
}

void SingleChannel::stopDAQ() {
    foreach(QTimer *tm, m_timer_pool) {
        tm->stop();
    }
}

void SingleChannel::run()
{
    QSerialPort serial;

    serial.setPortName(m_ch.m_portName);

    serial.setBaudRate(m_ch.m_baudRate);

    serial.setParity(m_ch.m_parity);

    serial.setDataBits(m_ch.m_dataBits);

    serial.setStopBits(m_ch.m_stopBits);

    if (!serial.open(QIODevice::ReadWrite)) {
        //Todo: emit a signal here

        return;
    }

    while(!m_stop) {
        if (!m_packet_queue.isEmpty()) {
            Packet* pac = m_packet_queue.dequeue();

            qDebug() << pac->m_query_bytes;

            serial.write(pac->m_query_bytes);
            if (serial.waitForBytesWritten(1000)) {
                qDebug() << "data sent";
            }


            if (serial.waitForReadyRead(1000)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(50))
                    responseData += serial.readAll();

                qDebug() << "received";
                qDebug() << responseData;

                pac->m_response_bytes = responseData;
                //m_device_pool[0]->parseRxData(pac->m_response_bytes, 0);

                DataProcessor *dataProc = new DataProcessor(pac);

                connect(dataProc, &DataProcessor::sendData, this, &SingleChannel::getData);

                QThreadPool::globalInstance()->start(dataProc);
                //dataWorker = new DataProcessor();
                //dataWorker->processData(pac->m_response_bytes, m_device_pool[0]);
                // use queue to process data find related device, parse data and save data to db

                delete pac;
            } else {
                qDebug() << "time out, no data received";
            }
        }
        msleep(1);
    }
}

void SingleChannel::getData(QVector<DeviceData> data) {
    qDebug() << "get data from thread";
    emit sendData(data, m_ch_id);
}
