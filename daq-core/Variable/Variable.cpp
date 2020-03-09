#include "Variable.h"

#include <QRegularExpression>
#include "Models.h"
#include <memory>
#include <QDebug>
#include "UnitAndConversion.h"

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
        if (currentData.canConvert<double>())
        {
            prop["RealValue"] = currentData.toDouble();
        }
        else if (currentData.canConvert<QString>()) {
            prop["StringValue"] = currentData.toString();
        }
        prop["TimeStamp"] = this->currentTimeStamp;

        this->setSingleProperty("CurrentValue",ret);

        this->setSingleProperty("CurrentTimeStamp",this->currentTimeStamp);

        Models::instance().mVariableModel->updateVariable(*this);

        this->addDataToVariable(prop);

        toCalculate.clear();
    }
}

