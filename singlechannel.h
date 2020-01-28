#ifndef SINGLECHANNEL_H
#define SINGLECHANNEL_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTimer>
#include <QQueue>
#include <device.h>
#include <packet.h>
#include <dataprocessor.h>

class SingleChannel : public QThread
{
    Q_OBJECT

public:
    explicit SingleChannel(QObject *parent = nullptr);
    ~SingleChannel() override;

    void addDevice(Device* dev); // add device to current channel

    void startChannel();

    void stopChannel();

    void startDAQ();
    void stopDAQ();

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

    DataProcessor* dataWorker;

    int m_ch_id;

signals:
    void sendData(currentData data, int ch_id);

private slots:
    void getData(currentData data);
};

#endif // SINGLECHANNEL_H
