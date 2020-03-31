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


}


void VariableGroupModel::startDAQ(const QModelIndex& index, int simulation) {
    if (!isIndexValid(index)) {
        return;
    }
    VariableGroup& VariableGroup = *mVariableGroups->at(index.row());

    VariableGroup.startDAQ(simulation);

}

void VariableGroupModel::endDAQ(const QModelIndex& index, int simulation) {
    if (!isIndexValid(index)) {
        return;
    }
    VariableGroup& VariableGroup = *mVariableGroups->at(index.row());

    VariableGroup.endDAQ(simulation);

}

void VariableGroupModel::endAllDAQ(int simulation) {
    foreach(const shared_ptr<VariableGroup>& varG, (*mVariableGroups)) {
        varG->endDAQ(simulation);
    }
}


bool VariableGroupModel::findVariableGroupByID(int varGroupID, shared_ptr<VariableGroup>& varGroup)
{
    foreach(const shared_ptr<VariableGroup>& varG, (*mVariableGroups)) {
        if ((*varG).m_id == varGroupID) {
            varGroup = varG;
            return true;
        }
    }

    return false;
}

void VariableGroupModel::resolveDependency(){
    mVariableModel->resolveDependency();
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

bool VariableGroupModel::loadGroupsFromConfigFile(QString configFilePath)
{
    QFile loadFile(configFilePath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
           qWarning("Couldn't open save file.");
           return false;
    }

   QByteArray dataRead = loadFile.readAll();

   loadFile.close();

   QJsonDocument loadDoc(QJsonDocument::fromJson(dataRead));


   QJsonObject json = loadDoc.object();

   if (json.contains("StationName")) {
       mStationName = json["StationName"].toString();
   }
   else {
       return false;
   }

   if (json.contains("Type")){
       mStationType = json["Type"].toString();
   }
   else {
       return false;
   }

   if (json.contains("Version")) {
       mStationVersion = json["Version"].toDouble();
   }


   if (json.contains("Devices")) {
       QJsonArray deviceArr = json["Devices"].toArray();

       for (int i = 0; i < deviceArr.size(); i++) {
            QJsonObject singleDevice = deviceArr[i].toObject();

            QHash<QString, QVariant> properties;

            properties["Name"] = singleDevice["Name"].toString();
            properties["NodeID"] = singleDevice["Node"].toInt();
            properties["Protocol"] = singleDevice["Protocol"].toString();
            properties["ChannelID"] = -1;

            Models::instance().mDeviceModel->addDevice(properties);

            emit updateProgress("Create Device",i,deviceArr.size());
       }
   }


   if (json.contains("SubStations")) {
        QJsonArray pArr = json["SubStations"].toArray();

        for (int i = 0; i < pArr.size(); i++) {

            emit updateProgress("Create Sub Station",i,pArr.size());

            QJsonObject singleGroup = pArr[i].toObject();

            QHash<QString,QVariant> properties;


            foreach(const QString& key, singleGroup.keys()) {
                QJsonValue value = singleGroup.value(key);

                if (key != "SupplementsToEqautions")
                properties.insert(key,value.toVariant());
            }

            if (singleGroup.contains("SupplementsToEqautions")) {
                QJsonArray suppArr = singleGroup["SupplementsToEqautions"].toArray();

                QVector<QHash<QString,QVariant>> equation_supplements_list;

                 for (int j = 0; j < suppArr.size(); j++) {
                        QJsonObject singleEqnObj = suppArr[j].toObject();

                        QHash<QString,QVariant> singleEqnSup;

                        foreach(const QString&key, singleEqnObj.keys()) {
                            singleEqnSup.insert(key, singleEqnObj[key].toVariant());
                        }

                        equation_supplements_list.append(singleEqnSup);
                 }

                addVariableGroup(properties, equation_supplements_list);
            }



        }
   }

   return true;
}

void VariableGroupModel::addVariables(QHash<QString,QVariant> properties, QVector<QHash<QString,QVariant>> specific_properties) {
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

            mVariableModel->resolveFirstTime(properties["VariableGroupID"].toInt());
       }
    }
}

QModelIndex VariableGroupModel::addVariableGroup(QHash<QString,QVariant> properties, QVector<QHash<QString,QVariant>> specific_properties)
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

void VariableGroupModel::removeAllRows(const QModelIndex& parent) {
    beginRemoveRows(parent, 0, rowCount() - 1);
    mDb.variableGroupDao.removeAll();
    mVariableGroups->clear();
    endRemoveRows();
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
