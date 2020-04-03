#include "Variable.h"

#include <QRegularExpression>
#include "Models.h"
#include <memory>
#include <QDebug>
#include <QThreadPool>
#include "UnitAndConversion.h"
#include <Thread/ThreadCalculationProcessor.h>
#include <cmath>

using namespace std;

Variable::Variable(int id, QHash<QString,QVariant> properties, QVector<QHash<QString,QVariant>> group_properties):
            GenericDefinition(id,properties)
{
    this->setProperty("ObjType","variable");

    if (m_properties.contains("CurrentValue")) {
        currentData = m_properties["CurrentValue"];
    }


    foreach(auto& key, m_properties.keys()) {
        if (key == "Name")
        {
            foreach(auto& prop, group_properties) {
                if (prop["Name"].toString() == m_properties[key].toString()) {
                    foreach(auto &key, prop.keys()) {
                        if (m_properties.keys().contains(key)) {
                            m_properties[key] = prop[key];
                        }
                        m_properties.insert(key, prop[key]);
                    }
                }
            }
        }
    }

}

double Variable::getAverageDataByDataSize(int data_size) {

    double sum = 0;
    int count = 0;

    if (this->getSingleProperty("DataType").toString() == "string") {
        return 0;
    }
    else {
        for(auto it = historyData.end() - data_size; it != historyData.end(); it++) {
            QHash<QString,QVariant> da = qvariant_cast<QHash<QString,QVariant>>(*it);
            sum += da["Value"].toDouble();
            count++;
        }
    }

    double avg = sum / (double)count;

    return avg;

}

double calculateSD(QVector<double> data)
{
    if (data.size() == 0) {
        return 0;
    }

    double sum = 0.0, mean, standardDeviation = 0.0;


    int i;

    for(i = 0; i < data.size(); ++i)
    {
        sum += data.at(i);
    }

    mean = sum / (double) data.size();

    for(i = 0; i < data.size(); ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / (double) data.size());
}

double Variable::getSTDEVByTimePeriod(int seconds) {

    if (seconds == 0) {
        return 0;
    }

    if (this->getSingleProperty("DataType").toString() == "string") {
        return 0;
    }


    if (historyData.size() == 1 || historyData.size() == 0) {
        return this->getSingleProperty("CurrentValue").toDouble();
    }

    int time_diff = 0;

    auto end_it = historyData.end() - 1;

    auto it = historyData.end();

    QHash<QString,QVariant> end_da = qvariant_cast<QHash<QString,QVariant>>(*end_it);

    QDateTime end_time = end_da["TimeStamp"].toDateTime();

    QVector<double> tmp_data;


    while(time_diff < seconds && it != historyData.begin()) // search for data size
    {
       it--;

       QHash<QString,QVariant> da = qvariant_cast<QHash<QString,QVariant>>(*it);

       time_diff = da["TimeStamp"].toDateTime().secsTo(end_time);

       tmp_data.append(da["Value"].toDouble());

    }

    return calculateSD(tmp_data);

}

double Variable::getAverageDataByTimePeriod(int seconds) {

    if (seconds == 0) {
        return 0;
    }

    if (this->getSingleProperty("DataType").toString() == "string") {
        return 0;
    }


    if (historyData.size() == 1 || historyData.size() == 0) {
        return this->getSingleProperty("CurrentValue").toDouble();
    }

    int time_diff = 0;

    auto end_it = historyData.end() - 1;

    auto it = historyData.end();

    QHash<QString,QVariant> end_da = qvariant_cast<QHash<QString,QVariant>>(*end_it);

    QDateTime end_time = end_da["TimeStamp"].toDateTime();

    double sum = 0;

    int count = 0;

    while(time_diff < seconds && it != historyData.begin()) // search for data size
    {
       it--;

       QHash<QString,QVariant> da = qvariant_cast<QHash<QString,QVariant>>(*it);

       time_diff = da["TimeStamp"].toDateTime().secsTo(end_time);

       sum += da["Value"].toDouble();

       count++;

    }

    double average = sum / (double)count;

    return average;

}

void Variable::addDataToVariable(QHash<QString,QVariant> data, int isInit)
{
    currentData = data["Value"];
    currentTimeStamp = data["TimeStamp"].toDateTime();

    QHash<QString,QVariant> prop;

    prop["VariableID"] = this->getSingleProperty("id");
    prop["Value"] = data["Value"];
    prop["Type"] = data["VariableType"];
    if (currentData.canConvert<double>())
    {
        prop["RealValue"] = currentData.toDouble();
    }
    else if (currentData.canConvert<QString>()) {
        prop["StringValue"] = currentData.toString();
    }
    prop["TimeStamp"] = currentTimeStamp;

    if (this->getSingleProperty("Log").toInt() == 1 && isInit == 1)  { // save to db

        Models::instance().mDataModel->addData(prop);
    }

    if (isInit == 1) {
        if (historyData.size() < MAXMEMORYDATASIZE){
            historyData.push_back(prop);
        } else {
            historyData.pop_front();
            historyData.push_back(prop);
        }
    }

    if (!this->requiredBy.empty()) {
//        foreach(auto& rev, requiredBy) {
            //qDebug() << "emit requiredby signal to var id=" << rev->m_id;
            emit sendDataToRequiredBy(prop);
//        }
    }

    emit sendDataToChart(prop);
}



void Variable::getDataFromRequired(QHash<QString,QVariant> data)
{

    qDebug() << "***** getDataFromRequired, incoming data ******";

    qDebug() << "receiver/current varID=" << m_id;

    qDebug() << "incoming VariableID=" << data["VariableID"].toInt();

    qDebug() << "Value=" << data["Value"];

    qDebug() << "Type=" << data["Type"];

    qDebug() << "***** getDataFromRequired, end ******";

    toCalculate[data["VariableID"].toInt()] = data["Value"];

    if (toCalculate.size() >= (int)required.size() || data["Type"].toString() == "UserInput") {
        // emit
        emit calculateVariable(data);

        //ThreadCalculationProcessor *calProc = new ThreadCalculationProcessor(*this,data);

        //QThreadPool::globalInstance()->start(calProc);
    }

}

