#include "ThreadChannel.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QThreadPool>
#include "Database/DatabaseManager.h"
#include <QSerialPort>

using namespace std;

ThreadChannel::ThreadChannel(Channel& ch, QObject *parent) :
     QThread(parent),
     m_ch_id(ch.m_id),
     m_ch(ch),
     m_stop(false)
{
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
    qDebug() << "thread channel " << m_ch_id << " created";
}


ThreadChannel::~ThreadChannel()
{
    qDebug() << "thread channel " << m_ch_id << " destroyed";
    //DatabaseManager::instance().removeChannel(m_ch_id);
    m_stop = true;


}




void ThreadChannel::addDevice(const shared_ptr<Device>& dev) {
    m_device_pool.push_back(dev);

    foreach(Command cmd, dev->m_commands) {
        if (cmd.isDAQ) {
            QTimer *m_timer = new QTimer(dev.get());

            m_timer->setInterval(cmd.interval);
            connect(m_timer, &QTimer::timeout,this,[=]() {
                Packet *pac = new Packet(dev);
                pac->m_packet_id = rand();
                pac->m_query_bytes = dev->buildQueryCmd(cmd);
                pac->m_cmd_id = cmd.cmd_id;
                qDebug() << "enqueu";
                m_packet_queue.enqueue(pac);
            });
            dev->m_timer_pool.append(m_timer);
        }
    }
}

void ThreadChannel::removeDeviceFromChannel(int dev_id) {

    for (auto it = m_device_pool.begin(); it != m_device_pool.end(); it++) {
        if ((*(*it)).getSingleProperty("id").toInt() == dev_id) {
            foreach(auto timer, (*(*it)).m_timer_pool) {
                timer->stop();
                timer->deleteLater();
            }
            (*(*it)).m_timer_pool.clear();
            m_device_pool.erase(it);
            break;
        }
    }
}

void ThreadChannel::startChannel() {
    if (!isRunning()){

        qDebug() << "channel started";
        m_stop = false;
        start();
        startDAQ();
    }
}


void ThreadChannel::stopChannel() {
    if (isRunning()) {
        qDebug() << "channel stoped";
        m_stop = true;
        stopDAQ();
        //quit();
    }
}

void ThreadChannel::startDAQ() {
    foreach(auto& dev, m_device_pool) {
        foreach(QTimer* tm, dev->m_timer_pool) {
            tm->start();
        }
    }
}

void ThreadChannel::stopDAQ() {
    foreach(auto& dev, m_device_pool) {
        foreach(QTimer* tm, dev->m_timer_pool) {
            tm->stop();
        }
    }
}

void ThreadChannel::run()
{
    QSerialPort serial;

    serial.setPortName(m_ch.getSingleProperty("ComPort").toString());

    serial.setBaudRate(m_ch.getSingleProperty("BaudRate").toInt());

    // To do

    connect(&serial,&QSerialPort::errorOccurred,this,&ThreadChannel::stopChannel);

//    serial.setParity(m_ch.getProperty("Parity").toString());

//    serial.setDataBits(m_ch.getProperty("DataBits").toString());

//    serial.setStopBits(m_ch.getProperty("StopBits").toString());

    if (!serial.open(QIODevice::ReadWrite)) {
        //Todo: emit a signal here
        qDebug() << serial.errorString();
    }

    while(!m_stop) {
        if (!m_packet_queue.isEmpty()) {
            Packet* pac = m_packet_queue.dequeue();

            qDebug() << pac->m_query_bytes;

            serial.write(pac->m_query_bytes);
            if (serial.waitForBytesWritten(500)) {
                qDebug() << "data sent";
            }


            if (serial.waitForReadyRead(500)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(50))
                    responseData += serial.readAll();

                qDebug() << "received";
                qDebug() << responseData;

                pac->m_response_bytes = responseData;
                //m_device_pool[0]->parseRxData(pac->m_response_bytes, 0);

                ThreadDataProcessor *dataProc = new ThreadDataProcessor(pac);

                //connect(dataProc, &ThreadDataProcessor::sendData, this, &ThreadChannel::getData);

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
