#include "singlechannel.h"
#include <QSerialPort>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SingleChannel::SingleChannel(QObject *parent) :
    QThread(parent)
{
    Device alicat, alicat1;

    alicat.loadFromConfig("LFE","ALICAT",0);

    qDebug() << alicat.m_daq_cmd;

//    alicat.m_daq_cmd.clear();

//    alicat.m_daq_cmd.append(0x8E);
//    alicat.m_daq_cmd.append(0x04);
//    alicat.m_daq_cmd.append((uint8_t)0x00);
//    alicat.m_daq_cmd.append((uint8_t)0x00);
//    alicat.m_daq_cmd.append(0x01);
//    alicat.m_daq_cmd.append(0xFE);

    alicat1.node_str = "B";

    addDevice(alicat);
//    loadProtocol("ASCII_ALICAT");
//    m_timer1 = new QTimer(this);
//    m_timer2 = new QTimer(this);
//    m_timer1->setInterval(2000);
//    m_timer2->setInterval(10000);

//    connect(m_timer1, SIGNAL(timeout()), this, SLOT(trigger1()));
//    connect(m_timer2, SIGNAL(timeout()), this, SLOT(trigger2()));

//    m_timer1->start();
//    //m_timer2->start();
}


SingleChannel::~SingleChannel()
{

}

void SingleChannel::addDevice(Device dev) {
    m_devicePool.append(dev);

    // setup DAQ timer
    QTimer *m_timer = new QTimer(this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, this, [=]() {
        Packet *pac = new Packet();
        pac->m_packet_id = rand();
        pac->m_query_bytes = dev.m_daq_cmd;
        m_packet_queue.enqueue(pac);
    });
    m_timer->start();
    m_timerPool.append(m_timer);
}

void SingleChannel::loadProtocol(QString protocolName){
    QFile loadFile(":/protocol/all.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    QByteArray saveData = loadFile.readAll();

    loadFile.close();

    QJsonDocument  loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    QString proName, queryStr, regexStr;

    if (json.contains("ProtocolName") && json["ProtocolName"].isString()) {
        proName = json["ProtocolName"].toString();
    }

    if (json.contains("DataQueryString") && json["DataQueryString"].isString()) {
        queryStr = json["DataQueryString"].toString();
    }

    if (json.contains("DataResponseRegex") && json["DataResponseRegex"].isString()) {
        regexStr = json["DataResponseRegex"].toString();
    }


    qDebug() << json.count();

}

void SingleChannel::trigger1()
{

    m_dataQueue.enqueue("tr1_" + QString::number(rand() % 10 + 1));
    qDebug() << "trigger1";
}

void SingleChannel::trigger2()
{
    m_dataQueue.enqueue("tr2_" + QString::number(rand()));
    qDebug() << "trigger2";
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
                while (serial.waitForReadyRead(100))
                    requestData += serial.readAll();

                qDebug() << "received";
                qDebug() << requestData;

                pac->m_response_bytes = requestData;


                // use queue to process data find related device, parse data and save data to db

                delete pac;
            } else {
                qDebug() << "time out, no data received";
            }
        }
        msleep(1);
    }
}
