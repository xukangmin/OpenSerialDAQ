#include "VariableModel.h"
#include <QDebug>
#include <QSerialPortInfo>


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


QModelIndex VariableModel::addVariable(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<Variable> newVariable(new Variable(0, properties));
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
