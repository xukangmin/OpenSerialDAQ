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

void Variable::addDataToVariable(QVariant data)
{
    currentData = data;
    currentTimeStamp = QDateTime::currentDateTime();

    if (this->getSingleProperty("Log").toInt() == 1) { // save to db
        QHash<QString,QVariant> prop;

        prop["VariableID"] = this->getSingleProperty("id");
        prop["Value"] = data;
        prop["TimeStamp"] = currentTimeStamp;
        Models::instance().mDataModel->addData(prop);
    }

    if (!this->required.empty()) {
        foreach(auto& rev, required) {
            emit sendDataToRequiredBy(rev->m_id, data);
        }
    }
}

void Variable::getDataFromRequired(int var_id, QVariant data)
{
    if (toCalculate.size() == (int)required.size()) {
        // perform calculation and empty toCalculate

        qDebug() << this->getSingleProperty("Equation");

        toCalculate.clear();
    } else {
        toCalculate[var_id] = data;
    }
}

