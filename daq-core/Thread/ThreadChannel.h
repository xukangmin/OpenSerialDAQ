#ifndef SINGLECHANNEL_H
#define SINGLECHANNEL_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTimer>
#include <QQueue>
#include "Device/Device.h"
#include "Packet/Packet.h"
#include "ThreadDataProcessor.h"
#include "Channel/Channel.h"

class ThreadChannel : public QThread
{
    Q_OBJECT

public:
    explicit ThreadChannel(Channel& ch, QObject *parent = nullptr);
    ~ThreadChannel() override;

    void addDevice(Device* dev); // add device to current channel


    int m_ch_id;
    Channel& m_ch;
private:

    QObject* par;
    bool m_stop;
    void run() override;
    QTimer* m_timer1;
    QTimer* m_timer2;
    QMutex m_mutex;
    QQueue<Packet*> m_packet_queue;
    QQueue<QString> m_dataQueue;
    void loadProtocol(QString protocolName);

    QVector<QTimer*> m_timer_pool;

    QVector<Device*> m_device_pool;

    ThreadDataProcessor* dataWorker;

public slots:
    void removeDeviceFromChannel(int dev_id);


signals:
    void sendData(QVector<DeviceData> data, int ch_id);

public slots:
    void getData(QVector<DeviceData> data);
    void startChannel();
    void stopChannel();

    void startDAQ();
    void stopDAQ();

};

#endif // SINGLECHANNEL_H
