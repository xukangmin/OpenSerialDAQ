#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H


#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <device.h>

class DataProcessor : public QThread
{

Q_OBJECT

public:
    explicit DataProcessor(QObject *parent = nullptr);
    ~DataProcessor() override;

    void run() override;

    void processData(QByteArray data, Device *dev);

private:
    Device* m_dev;
    QByteArray m_rx_data;
};

#endif // DATAPROCESSOR_H
