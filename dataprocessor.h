#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H


#include <QRunnable>
#include <device.h>
#include <packet.h>
#include "devicedata.h"

class DataProcessor : public QObject, public QRunnable
{

Q_OBJECT

public:
    DataProcessor(Packet* pac);
    ~DataProcessor();

    void run() override;

signals:
    void sendData(QVector<DeviceData> data);

private:
    Device* m_dev;
    QByteArray m_rx_data;
    int m_cmd_id;
};

#endif // DATAPROCESSOR_H
