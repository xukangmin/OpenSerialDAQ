#include "dataprocessor.h"
#include <QDebug>
#include <QMutex>
#include "databasemanager.h"

QMutex mutex;

DataProcessor::DataProcessor(Packet* pac) :
    m_dev(pac->dev),
    m_rx_data(pac->m_response_bytes),
    m_cmd_id(pac->m_cmd_id)
{

}


DataProcessor::~DataProcessor()
{
    qDebug() << "data process thread destroyed";
}

//void DataProcessor::processData(QByteArray data, Device *dev) {
//        m_rx_data = data;
//        m_dev = dev;
//        run();
//}

void DataProcessor::run() {



    qDebug() << "data process started";

    QVector<DeviceData> parData;

    parData = m_dev->parseRxData(m_rx_data, m_cmd_id);
    // log to database


    mutex.lock();
    if (!parData.empty()) {
        for(auto da : parData) {
            DatabaseManager::instance().insertData(da, m_dev->m_device_id);
        }
    }
    mutex.unlock();

    //data.values = m_dev->m_devData
    //data.timestamp = m_dev->m_deviceData.currentTimeStamp;
    //data.name = m_dev->m_deviceData.dataName;
    //data.unit = m_dev->m_deviceData.dataUnit;
    // send to front end
    emit sendData(parData);

    //m_dev->parseRxData(m_rx_data, 0);
}

