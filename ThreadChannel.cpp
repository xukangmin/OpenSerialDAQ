#include "ThreadChannel.h"
#include <QSerialPort>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThreadPool>
#include "MainWindow.h"
#include "DatabaseManager.h"

ThreadChannel::ThreadChannel(Channel ch, QObject *parent) :
     QThread(parent),
     m_ch(ch),
     m_stop(false),
     m_ch_id(ch.m_id)
{
}


ThreadChannel::~ThreadChannel()
{
    qDebug() << "channel " << m_ch_id << " destroyed";
    //DatabaseManager::instance().removeChannel(m_ch_id);
    m_stop = true;


}

void ThreadChannel::addDevice(Device* dev) {
    m_device_pool.append(dev);

    foreach(Command cmd, dev->m_commands) {
        if (cmd.isDAQ) {
            QTimer *m_timer = new QTimer(dev);

            m_timer->setInterval(cmd.interval);
            connect(m_timer, &QTimer::timeout,this,[=]() {
                Packet *pac = new Packet();
                pac->m_packet_id = rand();
                pac->m_query_bytes = dev->buildQueryCmd(cmd);
                pac->dev = dev;
                pac->m_cmd_id = cmd.cmd_id;
                qDebug() << "enqueu";
                m_packet_queue.enqueue(pac);
            });
            dev->m_timer_pool.append(m_timer);
        }
    }
}

void ThreadChannel::removeDeviceFromChannel(int dev_id) {
    foreach(Device *dev, m_device_pool) {
        if (dev->m_device_id == dev_id) {
            foreach(QTimer* tm, dev->m_timer_pool) {
                tm->stop();
            }
            m_device_pool.removeOne(dev);
            dev->deleteLater();
        }
    }
}

void ThreadChannel::startChannel() {
    if (!isRunning()){
        m_stop = false;
        start();
        startDAQ();
    }
}


void ThreadChannel::stopChannel() {
    if (isRunning()) {
        m_stop = true;
        stopDAQ();
        //quit();
    }
}

void ThreadChannel::startDAQ() {
    foreach(Device* dev, m_device_pool) {
        foreach(QTimer* tm, dev->m_timer_pool) {
            tm->start();
        }
    }
}

void ThreadChannel::stopDAQ() {
    foreach(Device* dev, m_device_pool) {
        foreach(QTimer* tm, dev->m_timer_pool) {
            tm->stop();
        }
    }
}

void ThreadChannel::run()
{
    QSerialPort serial;

    serial.setPortName(m_ch.m_portName);

    serial.setBaudRate(m_ch.m_baudRate);

    serial.setParity(m_ch.m_parity);

    serial.setDataBits(m_ch.m_dataBits);

    serial.setStopBits(m_ch.m_stopBits);

    qDebug() << "channel started";

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

                ThreadDataProcessor *dataProc = new ThreadDataProcessor(pac);

                connect(dataProc, &ThreadDataProcessor::sendData, this, &ThreadChannel::getData);

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

    serial.close();
}

void ThreadChannel::getData(QVector<DeviceData> data) {
    qDebug() << "get data from thread";
    emit sendData(data, m_ch_id);
}
