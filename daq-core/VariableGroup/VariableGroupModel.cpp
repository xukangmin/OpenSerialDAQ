#include "VariableGroupModel.h"
#include <Models.h>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSerialPortInfo>


using namespace std;

VariableGroupModel::VariableGroupModel(VariableModel *variableModel, QObject* parent) :
    QAbstractTableModel(parent),
    mDb(DatabaseManager::instance()),
    mVariableGroups(mDb.variableGroupDao.variablegroups()),
    mVariableModel(variableModel)
{
    foreach(auto& vgrp, *mVariableGroups) {
        mVariableModel->resolveDependency(vgrp->m_id);
    }

}


bool VariableGroupModel::isVariableGroupExists(QHash<QString,QVariant> property)
{
    auto it = find_if(mVariableGroups->begin(),mVariableGroups->end(),[&property](shared_ptr<VariableGroup>& var) {
            if (var->getSingleProperty("Name") == property["Name"] &&
                var->getSingleProperty("DeviceID") == property["DeviceID"])
    {
        return true;
    } else {
        return false;
    }
});

    if (it != mVariableGroups->end()) {
        return true;
    } else {
        return false;
    }

}

void VariableGroupModel::loadGroupsFromConfigFile(QString configFilePath)
{
    QFile loadFile(configFilePath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
           qWarning("Couldn't open save file.");
    }

   QByteArray dataRead = loadFile.readAll();

   loadFile.close();

   QJsonDocument loadDoc(QJsonDocument::fromJson(dataRead));


   QJsonObject json = loadDoc.object();

   if (json.contains("Station")) {
        QJsonArray pArr = json["Station"].toArray();

        for (int i = 0; i < pArr.size(); i++) {
            QJsonObject singleGroup = pArr[i].toObject();

            QHash<QString,QVariant> properties;

            QHash<QString,QVariant> specific_properties;

            foreach(const QString& key, singleGroup.keys()) {
                QJsonValue value = singleGroup.value(key);

                if (key != "Values")
                properties.insert(key,value.toVariant());
            }

            if (singleGroup.contains("Values")) {
                QJsonObject valueObj = singleGroup["Values"].toObject();

                foreach(const QString&key, valueObj.keys()) {
                    specific_properties.insert(key, valueObj[key].toVariant());
                }

            }


            addVariableGroup(properties, specific_properties);
        }
   }
}

void VariableGroupModel::addVariables(QHash<QString,QVariant> properties, QHash<QString,QVariant> specific_properties) {
    QString equation_to_load;

    if (properties.contains("EquationTempelate")) {
        equation_to_load = properties["EquationTempelate"].toString();
        QFile loadFile(":/Equation" + equation_to_load + ".json");

        if (!loadFile.open(QIODevice::ReadOnly)) {
               qWarning("Couldn't open save file.");
        }

        QByteArray dataJson = loadFile.readAll();

       loadFile.close();

        QJsonDocument loadDoc(QJsonDocument::fromJson(dataJson));

       QJsonObject json = loadDoc.object();

       if (json.contains("Equations")) {
            QJsonArray pArr = json["Equations"].toArray();

            for (int i = 0; i < pArr.size(); i++) {
                QJsonObject singleEquation = pArr[i].toObject();

                QHash<QString,QVariant> var_properties;

                foreach(const QString& key, singleEquation.keys()) {
                    QJsonValue value = singleEquation[key];
                    var_properties.insert(key, value.toVariant());
                }
                var_properties["VariableGroupID"] = properties["VariableGroupID"];
                mVariableModel->addVariable(var_properties, specific_properties);

            }

            mVariableModel->resolveDependency(properties["VariableGroupID"].toInt());
       }
    }
}

QModelIndex VariableGroupModel::addVariableGroup(QHash<QString,QVariant> properties, QHash<QString,QVariant> specific_properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    unique_ptr<VariableGroup> newVariableGroup(new VariableGroup(0, properties, specific_properties));
    mDb.variableGroupDao.addVariableGroup(*newVariableGroup);
    (*newVariableGroup).setSingleProperty("VariableGroupID",(*newVariableGroup).m_id);
    addVariables((*newVariableGroup).m_properties, (*newVariableGroup).m_group_properties);
    mVariableGroups->push_back(move(newVariableGroup));
    endInsertRows();
    return index(rowIndex, 0);
}

int VariableGroupModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return VariableGroupColumnSize;
}

int VariableGroupModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mVariableGroups->size();
}

QVariant VariableGroupModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }
    const VariableGroup& VariableGroup = *mVariableGroups->at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return VariableGroup.m_properties[VariableGroupHeaderList[1]];
        case Roles::IdRole:
            return VariableGroup.m_properties[VariableGroupHeaderList[0]];
        case Roles::NameRole:
            return VariableGroup.m_properties[VariableGroupHeaderList[1]];
        case Roles::DataRole:
            return VariableGroup.m_properties[VariableGroupHeaderList[index.column()]];
        default:
            return QVariant();
    }

}


bool VariableGroupModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    VariableGroup& VariableGroup = *mVariableGroups->at(index.row());
//    VariableGroup.m_portName =  value.toString();
    mDb.variableGroupDao.updateVariableGroup(VariableGroup);
    emit dataChanged(index, index);
    return true;
}

bool VariableGroupModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const VariableGroup& VariableGroup = *mVariableGroups->at(row + countLeft);
        mDb.variableGroupDao.removeVariableGroup(VariableGroup.m_id);
    }
    mVariableGroups->erase(mVariableGroups->begin() + row, mVariableGroups->begin() + row + count);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> VariableGroupModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool VariableGroupModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}
