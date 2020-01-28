#include "dataprocessor.h"
#include <QDebug>

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

    m_dev->parseRxData(m_rx_data, m_cmd_id);

    // log to database



    currentData data;

    data.values = m_dev->m_deviceData.currentData;
    data.timestamp = m_dev->m_deviceData.currentTimeStamp;
    data.name = m_dev->m_deviceData.dataName;
    data.unit = m_dev->m_deviceData.dataUnit;
    // send to front end
    emit sendData(data);

    //m_dev->parseRxData(m_rx_data, 0);
}

