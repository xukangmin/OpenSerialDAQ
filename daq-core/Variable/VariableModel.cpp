#include "VariableModel.h"
#include <Models.h>
#include <QDebug>
#include <QRegularExpression>
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

            if (dev_model->data(idIndex,Roles::IdRole) == var->getSingleProperty("DeviceID")) {
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

    //updateVariable(*var);

    addDataToVariableModel(prop, 0);
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

QModelIndex VariableModel::getIndexByVariable(Variable &var)
{
    for(int i = 0; i < (*mVariables).size(); i++) {
        if ((*mVariables).at(i)->m_id == var.m_id) {
            return index(i,7);
        }
    }
}

void VariableModel::addDataToVariableModel(QHash<QString,QVariant> data, int isInit)
{
    for(auto i = 0; i < (*mVariables).size(); i++)
    {
        if ((*mVariables).at(i)->m_id == data["VariableID"].toInt())
        {
            (*mVariables).at(i)->addDataToVariable(data,isInit);
            setData(this->index(i,0),data,Roles::UpdateDataRole);
        }
    }
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

bool VariableModel::findVariablesByGroupID(int group_id, vector<shared_ptr<Variable>>& var_ret)
{

    foreach(const shared_ptr<Variable>& var, (*mVariables)) {
        if ((*var).getSingleProperty("VariableGroupID").toInt() == group_id){
            var_ret.push_back(var);
        }
    }

    return true;
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
    if (isIndexValid(index) && role == Roles::UpdateDataRole) {
        Variable& variable = *mVariables->at(index.row());

        QHash<QString,QVariant> inData = qvariant_cast<QHash<QString,QVariant>>(value);

        if (inData.contains("Value")) {
            inData["CurrentValue"] = inData["Value"];
        }

        if (inData.contains("TimeStamp")) {
            inData["CurrentTimeStamp"] = inData["TimeStamp"];
        }

        for(int i = 1; i < VariableColumnSize; i++) {
            if (inData.keys().contains(VariableHeaderList[i]))
            {
                variable.setSingleProperty(VariableHeaderList[i],inData[VariableHeaderList[i]]);
            }
        }

        mDb.variableDao.updateVariable(variable);
        emit dataChanged(index, this->index(index.row(),VariableColumnSize - 1));

        return true;
    }


    return false;
}

QVariant VariableModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if (/*role != Qt::DisplayRole || */orientation != Qt::Horizontal || section > columnCount())
        return QVariant();

    if (role == Qt::DisplayRole)
        return VariableHeaderList[section];

    return QVariant();
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
    return roles;
}

bool VariableModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}


VariableProxyModel::VariableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool VariableProxyModel::filterAcceptsRow(int sourceRow,
                                              const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 2, sourceParent);

    QVariant tmp = sourceModel()->data(index);

    return (tmp.toInt() == mGroupID);
}

bool VariableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if (source_column == 3 || source_column == 6 || source_column == 7 || source_column == 8)
    {
        return true;
    }
    else {
        return false;
    }


}

void VariableProxyModel::setGroupIndex(int groupIndex) {

    beginResetModel();

    QModelIndex in = Models::instance().mVariableGroupModel->index(groupIndex,0);

    mGroupID = Models::instance().mVariableGroupModel->data(in, VariableGroupModel::Roles::IdRole).toInt();

    endResetModel();
}


void VariableProxyModel::setDataByName(QString varName, QVariant inData)
{
    for(int i = 0; i < sourceModel()->rowCount(); i++) {

        QString varNameModel = sourceModel()->data(sourceModel()->index(i,3)).toString().toUpper();
        int varGroupID = sourceModel()->data(sourceModel()->index(i,2)).toInt();
        int varID = sourceModel()->data(sourceModel()->index(i,0)).toInt();
        if (varNameModel == varName.toUpper() &&
            varGroupID == mGroupID)
        {
            VariableModel* model = static_cast<VariableModel*>(sourceModel());

            QHash<QString,QVariant> data;

            data["VariableID"] = varID;
            data["Value"] = inData;
            data["TimeStamp"] = QDateTime::currentDateTime();

            model->addDataToVariableModel(data);
        }
    }

}

QVariant VariableProxyModel::getDataByName(QString varName) {

    for(int i = 0; i < sourceModel()->rowCount(); i++) {

        QString toCompare = sourceModel()->data(sourceModel()->index(i,3)).toString().toUpper();
        int toCom = sourceModel()->data(sourceModel()->index(i,2)).toInt();
        if (toCompare == varName.toUpper() &&
            toCom == mGroupID)
        {
             QModelIndex in_data = sourceModel()->index(i,7);
             return sourceModel()->data(in_data);
        }
    }

    return "N/A";
}

QVariant VariableProxyModel::data(const QModelIndex &index, int role) const noexcept
{

    QModelIndex in = sourceModel()->index(mapToSource(index).row(),mapToSource(index).column());

    switch (role) {
        case Qt::DisplayRole:
            return sourceModel()->data(in);
        default:
            return QVariant();
    }
}
