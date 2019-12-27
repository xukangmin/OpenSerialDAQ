#include "singlechannel.h"
#include <QSerialPort>
#include <QDebug>

SingleChannel::SingleChannel(QObject *parent) :
    QThread(parent)
{
    m_timer1 = new QTimer(this);
    m_timer2 = new QTimer(this);
    m_timer1->setInterval(10000);
    m_timer2->setInterval(10000);

    connect(m_timer1, SIGNAL(timeout()), this, SLOT(trigger1()));
    connect(m_timer2, SIGNAL(timeout()), this, SLOT(trigger2()));

    m_timer1->start();
    m_timer2->start();
}


SingleChannel::~SingleChannel()
{

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


            if (serial.waitForReadyRead(500)) {
                QByteArray requestData = serial.readAll();
                while (serial.waitForReadyRead(100))
                    requestData += serial.readAll();

                qDebug() << "received";
                qDebug() << requestData;

            } else {
                qDebug() << "time out, no data received";
            }
        }
        msleep(10);
    }
}
