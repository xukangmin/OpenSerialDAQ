#include "dataprocessor.h"

DataProcessor::DataProcessor(QObject *parent):
    QThread(parent)
{
}


DataProcessor::~DataProcessor()
{

}

void DataProcessor::processData(QByteArray data, Device *dev) {

    if (!isRunning()) {
        m_rx_data = data;
        m_dev = dev;
        start(LowPriority);
    }
}

void DataProcessor::run() {

    m_dev->parseRxData(m_rx_data, 0);

}

