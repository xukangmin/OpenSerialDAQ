#include "Variable.h"

#include <QRegularExpression>
#include "Models.h"
#include <memory>
#include <QDebug>

using namespace std;

Variable::Variable(int id, QHash<QString,QVariant> properties, QHash<QString,QVariant> group_properties):
            GenericDefinition(id,properties)
{
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

void Variable::addDataToVariable(QHash<QString,QVariant> data)
{
    currentData = data["Value"];
    currentTimeStamp = data["TimeStamp"].toDateTime();

    QHash<QString,QVariant> prop;

    prop["VariableID"] = this->getSingleProperty("id");
    prop["Value"] = data["Value"];
    if (currentData.canConvert<double>())
    {
        prop["RealValue"] = currentData.toDouble();
    }
    else if (currentData.canConvert<QString>()) {
        prop["StringValue"] = currentData.toString();
    }
    prop["TimeStamp"] = currentTimeStamp;

    if (this->getSingleProperty("Log").toInt() == 1) { // save to db

        Models::instance().mDataModel->addData(prop);
    }

    if (!this->requiredBy.empty()) {
        foreach(auto& rev, requiredBy) {
            emit sendDataToRequiredBy(prop);
        }
    }
}

void Variable::getDataFromRequired(QHash<QString,QVariant> data)
{
    toCalculate[data["VariableID"].toInt()] = data["Value"];

    if (toCalculate.size() == (int)required.size()) {
        // perform calculation and empty toCalculate
        QString eqn = this->getSingleProperty("Equation").toString();

        foreach(auto singleID, toCalculate.keys()) {
            eqn.replace("{" + QString::number(singleID) + "}",toCalculate[singleID].toString());
        }

        if (eqn.contains("UnitConv")) {
            // resolve unit conv

        }

        if (eqn.contains("Viscocity"))
        {

        }

        if (eqn.contains("ViscocityCF"))
        {

        }



        toCalculate.clear();
    }
}

