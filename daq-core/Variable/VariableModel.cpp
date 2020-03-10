#include "VariableModel.h"
#include <Models.h>
#include <QDebug>
#include <QSerialPortInfo>
#include <UnitAndConversion.h>


using namespace std;

VariableModel::VariableModel(DeviceModel* dev_model, QObject* parent) :
    QAbstractTableModel(parent),
    m_dev_model(dev_model),
    mDb(DatabaseManager::instance()),
    mVariables(mDb.variableDao.variables())
{
    // load variables from devices
    for(int i = 0; i < dev_model->rowCount(); i++) {
        auto propertList = dev_model->getVariableDefinitionFromDevice(dev_model->index(i,0));

        foreach(auto prop, propertList) {
            if (!isVariableExists(prop)) {
                addVariable(prop);
            }
        }
    }

    // add variables back to devices

    foreach(auto& var, *mVariables) {
        for(int i = 0; i < dev_model->rowCount(); i++) {
            QModelIndex idIndex = dev_model->index(i,0);

            if (dev_model->data(idIndex) == var->getSingleProperty("DeviceID")) {
                dev_model->addVariableToDevice(var,idIndex);
            }

        }
    }
}


bool VariableModel::isVariableExistsInVector(const shared_ptr<Variable>& t, vector<shared_ptr<Variable>>& v)
{
    auto it = find_if(v.begin(),v.end(),[&t](shared_ptr<Variable>& var)
    {
            return (var->getSingleProperty("id") == t->getSingleProperty("id"));
    });

    if (it != v.end()) {
        return true;
    } else {
        return false;
    }

}


bool VariableModel::isVariableExists(QHash<QString,QVariant> property)
{
    auto it = find_if(mVariables->begin(),mVariables->end(),[&property](shared_ptr<Variable>& var) {
            if (var->getSingleProperty("Name") == property["Name"] &&
                var->getSingleProperty("DeviceID") == property["DeviceID"])
    {
        return true;
    } else {
        return false;
    }
});

    if (it != mVariables->end()) {
        return true;
    } else {
        return false;
    }

}


bool VariableModel::calculateVariable(const shared_ptr<Variable>& var) {
    QString eqn = var->getSingleProperty("Equation").toString();

    foreach(auto singleID, var->toCalculate.keys()) {
        eqn.replace("{" + QString::number(singleID) + "}",var->toCalculate[singleID].toString());
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

        if (findVariableByID(id_tmp.toInt(),t))
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

    var->currentData = ret;

    QDateTime dt = QDateTime::currentDateTime();

    foreach(auto key, var->toCalculate.keys()){
        if (var->toCalculate[key].toDateTime() > dt) {
            dt = var->toCalculate[key].toDateTime();
        }
    }

    var->currentTimeStamp = dt;

    QHash<QString,QVariant> prop;

    prop["VariableID"] = var->getSingleProperty("id");
    prop["Value"] = ret;
    if (var->currentData.canConvert<double>())
    {
        prop["RealValue"] = var->currentData.toDouble();
    }
    else if (var->currentData.canConvert<QString>()) {
        prop["StringValue"] = var->currentData.toString();
    }
    prop["TimeStamp"] = var->currentTimeStamp;

    var->setSingleProperty("CurrentValue",ret);

    var->setSingleProperty("CurrentTimeStamp",var->currentTimeStamp);

    updateVariable(*var);

    //var->addDataToVariable(prop);

    var->toCalculate.clear();

    return true;
}

bool VariableModel::resolveDependency(int group_id)
{

    foreach(const shared_ptr<Variable>& var, (*mVariables)) {

        QString eqn = var->getSingleProperty("Equation").toString();

        var->equation = eqn;

        if (eqn != "N/A") {

            // first search {id} and bind
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

                if (findVariableByID(id_tmp.toInt(),t))
                {
                    if (t->getSingleProperty("Type").toString() == "UserInput" ||
                        t->getSingleProperty("Type").toString() == "RangeSpecific")
                    {
                        if (!isVariableExistsInVector(var,t->requiredBy)) {
                            t->requiredBy.push_back(var);
                            connect(t.get(),&Variable::sendDataToRequiredBy,var.get(),&Variable::getDataFromRequired);
                        }

                    }
                    else
                    {
                        if (!isVariableExistsInVector(var,t->requiredBy)) {
                            t->requiredBy.push_back(var);
                            connect(t.get(),&Variable::sendDataToRequiredBy,var.get(),&Variable::getDataFromRequired);
                        }

                        if (!isVariableExistsInVector(t,var->required)) {
                            var->required.push_back(t);
                        }


                    }
                }
            }


            // second search Device(), [ ]  and convert and bind
            QList<QString> matches;

            QRegularExpression reA("Device\\((.*?)\\)|\\[(.*?)\\]", QRegularExpression::CaseInsensitiveOption);

            QRegularExpressionMatchIterator i = reA.globalMatch(eqn);
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                if (match.hasMatch()) {
                     matches.append(match.captured(0));
                }
            }

            foreach(QString match, matches) {
                // find corresponding variable

                if (match.contains("Device")) {
                    // first find device id in device model
                    QString tmp_match = match;
                    tmp_match.remove("Device");
                    tmp_match.remove("(");
                    tmp_match.remove(")");

                    QStringList result = tmp_match.split(',');

                    int devid = m_dev_model->getDeviceIDByNameAndNode(result.at(0),result.at(1).toInt());

                    if (devid != -1) {
                        // find variable with name and dev id
                        shared_ptr<Variable> t;
                        if (findVariableByNameAndDeviceID(result.at(2),devid,t)) {
                            var->equation = var->equation.replace(match, "{" + QString::number(t->m_id) + "}");
                            var->setSingleProperty("Equation", var->equation);

                            if (!isVariableExistsInVector(var,t->requiredBy)) {
                                t->requiredBy.push_back(var);
                                connect(t.get(),&Variable::sendDataToRequiredBy,var.get(),&Variable::getDataFromRequired);
                            }

                            if (!isVariableExistsInVector(t,var->required)) {
                                var->required.push_back(t);
                            }
                        }
                    }
                } else { // find
                    QString tmp_match = match;
                    tmp_match.remove("[");
                    tmp_match.remove("]");
                    shared_ptr<Variable> t;
                    if (findVariableByNameAndGroupID(tmp_match,group_id,t)) {



                        if (t->getSingleProperty("Type").toString() == "UserInput")
                        {
                            var->equation = var->equation.replace(match, "{" + QString::number(t->m_id) + "}");
                            if (!isVariableExistsInVector(var,t->requiredBy)) {
                                t->requiredBy.push_back(var);
                                connect(t.get(),&Variable::sendDataToRequiredBy,var.get(),&Variable::getDataFromRequired);
                            }

                        }
                        else if (t->getSingleProperty("Type").toString() == "RangeSpecific")  // constant
                        {
                            var->equation = var->equation.replace(match, t->getSingleProperty("CurrentValue").toString());
                            if (!isVariableExistsInVector(var,t->requiredBy)) {
                                t->requiredBy.push_back(var);
                                connect(t.get(),&Variable::sendDataToRequiredBy,var.get(),&Variable::getDataFromRequired);
                            }

                        }
                        else
                        {
                            var->equation = var->equation.replace(match, "{" + QString::number(t->m_id) + "}");
                            if (!isVariableExistsInVector(var,t->requiredBy)) {
                                t->requiredBy.push_back(var);
                                connect(t.get(),&Variable::sendDataToRequiredBy,var.get(),&Variable::getDataFromRequired);
                            }

                            if (!isVariableExistsInVector(t,var->required)) {
                                var->required.push_back(t);
                            }
                        }

                    }
                }
            }

            // update
            var->setSingleProperty("Equation",var->equation);

            int const_count = 0;

            foreach(auto& v, var->required){
                if (v->getSingleProperty("Type").toString() == "RangeSpecific" &&
                    v->getSingleProperty("Type").toString() == "UserInput") {
                    const_count++;
                }
            }

            if (const_count == (int)var->required.size()) { // all constant, do calculation
                calculateVariable(var);
            }


            mDb.variableDao.updateVariable(*var);
        }





    }



    return true;
}

void VariableModel::addDataToVariable(QHash<QString,QVariant> data, shared_ptr<Variable> &var)
{
    var->addDataToVariable(data);
}

bool VariableModel::findVariableByNameAndDeviceID(QString name, int device_id, shared_ptr<Variable>& var_ret)
{

    foreach(const shared_ptr<Variable>& var, (*mVariables)) {
        if ((*var).getSingleProperty("Name").toString() == name &&
            (*var).getSingleProperty("DeviceID").toInt() == device_id){
            var_ret = var;
            return true;
        }
    }

    return false;
}

bool VariableModel::findVariableByID(int var_id, shared_ptr<Variable>& var_ret)
{

    foreach(const shared_ptr<Variable>& var, (*mVariables)) {
        if ((*var).m_id == var_id) {
            var_ret = var;
            return true;
        }
    }

    return false;
}

bool VariableModel::findVariableByNameAndGroupID(QString name, int group_id, shared_ptr<Variable>& var_ret)
{

    foreach(const shared_ptr<Variable>& var, (*mVariables)) {
        if ((*var).getSingleProperty("Name").toString() == name &&
            (*var).getSingleProperty("VariableGroupID").toInt() == group_id){
            var_ret = var;
            return true;
        }
    }

    return false;
}

void VariableModel::updateVariable(Variable& var) {
     mDb.variableDao.updateVariable(var);
}

QModelIndex VariableModel::addVariable(QHash<QString,QVariant> properties, QHash<QString,QVariant> group_properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Variable> newVariable(new Variable(0, properties, group_properties));
    mDb.variableDao.addVariable(*newVariable);
    mVariables->push_back(move(newVariable));
    endInsertRows();
    return index(rowIndex, 0);
}

int VariableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return VariableColumnSize;
}

int VariableModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mVariables->size();
}

QVariant VariableModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const Variable& Variable = *mVariables->at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return Variable.m_properties[VariableHeaderList[index.column()]];
        case Roles::IdRole:
            return Variable.m_properties[VariableHeaderList[0]];
        default:
            return QVariant();
    }

}


bool VariableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Variable& Variable = *mVariables->at(index.row());
//    Variable.m_portName =  value.toString();
    mDb.variableDao.updateVariable(Variable);
    emit dataChanged(index, index);
    return true;
}

bool VariableModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Variable& Variable = *mVariables->at(row + countLeft);
        mDb.variableDao.removeVariable(Variable.m_id);
    }
    mVariables->erase(mVariables->begin() + row, mVariables->begin() + row + count);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> VariableModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool VariableModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
