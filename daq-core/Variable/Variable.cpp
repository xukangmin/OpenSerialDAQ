#include "Variable.h"

#include <QRegularExpression>
#include "Models.h"
#include <memory>
#include <QDebug>
#include <QThreadPool>
#include "UnitAndConversion.h"
#include <Thread/ThreadCalculationProcessor.h>

using namespace std;

Variable::Variable(int id, QHash<QString,QVariant> properties, QHash<QString,QVariant> group_properties):
            GenericDefinition(id,properties)
{
    this->setProperty("ObjType","variable");

    if (m_properties.contains("CurrentValue")) {
        currentData = m_properties["CurrentValue"];
    }


    foreach(auto& key, m_properties.keys()) {
        if (key == "Name")
        {
            foreach(auto& keyg, group_properties.keys()){
                if (m_properties[key] == keyg) {
                    m_properties["CurrentValue"] = group_properties[keyg];
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
}

bool Variable::calculate(QHash<QString,QVariant> data) {


    // perform calculation and empty toCalculate
    QString eqn = this->getSingleProperty("Equation").toString();

    qDebug() << "start calculate " << "eqn = " << eqn;

    foreach(auto singleID, this->toCalculate.keys()) {
        eqn.replace("{" + QString::number(singleID) + "}",this->toCalculate[singleID].toString());
    }

    // resolve remaining var id values with current data
    QList<QString> id_matches;

    QRegularExpression id_reA("\\{.*?\\}", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator i_id = id_reA.globalMatch(eqn);
    while (i_id.hasNext()) {
        QRegularExpressionMatch id_match = i_id.next();
        if (id_match.hasMatch()) {
             id_matches.append(id_match.captured(0));
        }
    }

    foreach(QString m, id_matches)
    {
        QString id_tmp;
        id_tmp = m;
        id_tmp.remove("{");
        id_tmp.remove("}");

        shared_ptr<Variable> t;

        if (Models::instance().mVariableModel->findVariableByID(id_tmp.toInt(),t))
        {
            eqn.replace(m,t->getSingleProperty("CurrentValue").toString());
        }
    }


    QList<QString> matches;
    QList<QString> functionList;

    functionList = UnitAndConversion::instance().getFunctionNameList();

    QString reg_str = "";

    foreach(QString s, functionList) {
        reg_str += s + "\\((.*?)\\)|";
    }

    reg_str = reg_str.left(reg_str.length() - 1);

    QRegularExpression reA(reg_str, QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator i = reA.globalMatch(eqn);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
             matches.append(match.captured(0));
        }
    }

    foreach(auto m, matches) {
        QString inner;

        if (m.contains("UnitConv")) {
            inner = m;
            inner.remove("UnitConv");
            inner.remove("(");
            inner.remove(")");

            QList<QString> args;
            args = inner.split(',');

            double result = UnitAndConversion::instance().unitConvert(args.at(0).toDouble(), args.at(1), args.at(2));

            eqn.replace(m,QString::number(result));
        }
        else if (m.contains("ViscocityCF")) {
                inner = m;
                inner.remove("ViscocityCF");
                inner.remove("(");
                inner.remove(")");

                QList<QString> args;
                args = inner.split(',');

                double result = UnitAndConversion::instance().voscocityCF(args.at(0).toDouble(), args.at(1));

                eqn.replace(m,QString::number(result));
        }
        else if (m.contains("Viscocity")) {
                inner = m;
                inner.remove("Viscocity");
                inner.remove("(");
                inner.remove(")");

                QList<QString> args;
                args = inner.split(',');

                double result = UnitAndConversion::instance().voscocity(args.at(0).toDouble(), args.at(1));

                eqn.replace(m,QString::number(result));
        }
        else if (m.contains("MW")) {
                inner = m;
                inner.remove("MW");
                inner.remove("(");
                inner.remove(")");

                double result = UnitAndConversion::instance().MW(inner);

                eqn.replace(m,QString::number(result));
        }
    }

    qDebug() << eqn;

    double ret = UnitAndConversion::instance().evalSimpleEquation(eqn);

    this->currentData = ret;

    this->currentTimeStamp = data["TimeStamp"].toDateTime();

    QHash<QString,QVariant> prop;

    prop["VariableID"] = this->getSingleProperty("id");
    prop["Value"] = ret;
    if (this->currentData.canConvert<double>())
    {
        prop["RealValue"] = this->currentData.toDouble();
    }
    else if (this->currentData.canConvert<QString>()) {
        prop["StringValue"] = this->currentData.toString();
    }
    prop["TimeStamp"] = this->currentTimeStamp;

    this->setSingleProperty("CurrentValue",ret);

    this->setSingleProperty("CurrentTimeStamp",this->currentTimeStamp);

    Models::instance().mVariableModel->addDataToVariableModel(prop);

    //Models::instance().mVariableModel->updateVariable(*this);

    //this->addDataToVariable(prop);

    this->toCalculate.clear();

    return true;

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
        ThreadCalculationProcessor *calProc = new ThreadCalculationProcessor(*this,data);

        QThreadPool::globalInstance()->start(calProc);
    }

}

