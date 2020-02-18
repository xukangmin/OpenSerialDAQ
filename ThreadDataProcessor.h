#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H


#include <QRunnable>
#include <Device.h>
#include <Packet.h>
#include "DeviceData.h"

class ThreadDataProcessor : public QObject, public QRunnable
{

Q_OBJECT

public:
    ThreadDataProcessor(Packet* pac);
    ~ThreadDataProcessor();

    void run() override;

signals:
    void sendData(QVector<DeviceData> data);

private:
    Device* m_dev;
    QByteArray m_rx_data;
    int m_cmd_id;
};

#endif // DATAPROCESSOR_H
