#include "ThreadDataProcessor.h"
#include <QDebug>
#include <QMutex>
#include "Models.h"
#include <memory>
#include <vector>

using namespace std;

ThreadDataProcessor::ThreadDataProcessor(Packet* pac) :
    m_dev(pac->m_dev),
    m_rx_data(pac->m_response_bytes),
    m_cmd_id(pac->m_cmd_id)
{

}


ThreadDataProcessor::~ThreadDataProcessor()
{
    qDebug() << "data process thread destroyed";
}

//void DataProcessor::processData(QByteArray data, Device *dev) {
//        m_rx_data = data;
//        m_dev = dev;
//        run();
//}

void ThreadDataProcessor::run() {



    qDebug() << "data process started";

    auto parData = m_dev->parseRxData(m_rx_data, m_cmd_id);
    // log to database

    Models::instance().mutex_global.lock();


    if (!parData.empty()) {
        for(auto da : parData) {
            shared_ptr<Variable> var;
            Models::instance().mVariableModel->findVariableByID(da["VariableID"].toInt(),var);
            var->addDataToVariable(da);
        }
    }
    Models::instance().mutex_global.unlock();

    //data.values = m_dev->m_devData
    //data.timestamp = m_dev->m_deviceData.currentTimeStamp;
    //data.name = m_dev->m_deviceData.dataName;
    //data.unit = m_dev->m_deviceData.dataUnit;
    // send to front end
    //emit sendData(parData);

    //m_dev->parseRxData(m_rx_data, 0);
}

