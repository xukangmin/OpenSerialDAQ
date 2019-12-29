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

    alicat.node_str = "A";
    alicat1.node_str = "B";

    addDevice(alicat);
    addDevice(alicat1);
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
        m_dataQueue.enqueue(dev.node_str);
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

    serial.setPortName("COM97");
    serial.setBaudRate(QSerialPort::Baud9600);

    if (!serial.open(QIODevice::ReadWrite)) {
        //Todo: emit a signal here

        return;
    }

    while(1) {
        if (!m_dataQueue.isEmpty()) {
            QString req = m_dataQueue.dequeue();
            // send request
            req += "\r";

            const QByteArray reqData = req.toUtf8();
            serial.write(reqData);
            if (serial.waitForBytesWritten(1000)) {
                qDebug() << "data sent";
            }


            if (serial.waitForReadyRead(1000)) {
                QByteArray requestData = serial.readAll();
                while (serial.waitForReadyRead(100))
                    requestData += serial.readAll();

                qDebug() << "received";
                qDebug() << requestData;

            } else {
                qDebug() << "time out, no data received";
            }
        }
        msleep(1);
    }
}
