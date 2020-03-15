#include "DeviceModel.h"
#include <Models.h>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace std;

DeviceModel::DeviceModel(QObject* parent) :
    QAbstractItemModel(parent),
    mDb(DatabaseManager::instance()),
    mDevices(mDb.deviceDao.devices())
{
    this->setProperty("ObjType","root");
}

QList<QString> DeviceModel::getAvailableProtocols() {
    QFile loadFile(":/protocol.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    int dataIndex = 0;

    QByteArray saveData = loadFile.readAll();

    loadFile.close();

    QJsonDocument  loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    QString proName, queryStr, regexStr;

    QList<QString> protocols_list;

    if (json.contains("Protocols")) {
        QJsonObject protocols = json["Protocols"].toObject();
        QJsonArray pArr = json["Protocols"].toArray();
        QJsonObject singleProtocol;
        for(int i = 0; i < pArr.size(); i++){
           singleProtocol = pArr[i].toObject();

           if (singleProtocol.contains("ProtocolName"))
           {
               protocols_list.append(singleProtocol["ProtocolName"].toString());
           }

        }
    }

    return protocols_list;
}

bool DeviceModel::isDeviceExists(QString name, int node) {
    if (getDeviceIDByNameAndNode(name,node) == -1) {
        return false;
    } else {
        return true;
    }
}

int DeviceModel::getDeviceIDByNameAndNode(QString name, int node) {
    foreach(auto& dev, (*mDevices)) {
        if ((*dev).getSingleProperty("Name").toString() == name &&
            (*dev).getSingleProperty("NodeID").toInt() == node) {
            return (*dev).getSingleProperty("id").toInt();
        }
    }

    return -1;
}

void DeviceModel::addVariableToDevice(const std::shared_ptr<Variable>& var, const QModelIndex& dev_index)
{
    shared_ptr<Device>& device = mDevices->at(dev_index.row());
    device->mVariableList.push_back(var);
}

vector<QHash<QString, QVariant>> DeviceModel::getVariableDefinitionFromDevice(const QModelIndex& dev_index)
{
    shared_ptr<Device>& device = mDevices->at(dev_index.row());

    return device->mVariablePropertiesList;
}

QModelIndex DeviceModel::addDevice(QHash<QString,QVariant> properties)
{
    int rowIndex = rowCount();
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    shared_ptr<Device> newDevice(new Device(0, properties));

    foreach(auto prop, newDevice->mVariablePropertiesList) {
       // if (!Models::instance().mVariableModel->isVariableExists(prop)) {
            QModelIndex var_index = Models::instance().mVariableModel->addVariable(prop);

            shared_ptr<Variable> var;

            Models::instance().mVariableModel->findVariableByNameAndDeviceID(prop["Name"].toString(),newDevice->m_id,var);

            newDevice->mVariableList.push_back(var);
        //}
    }

    mDb.deviceDao.addDevice(*newDevice);
    mDevices->push_back(move(newDevice));
    endInsertRows();
    return index(rowIndex, 0);
}

void DeviceModel::addDeviceToChannel(const QModelIndex& dev_index, ChannelModel* ch_model,  const QModelIndex& ch_index)
{
    beginResetModel();
    qDebug() << "row=" << dev_index.row();
    shared_ptr<Device>& device = mDevices->at(dev_index.row());
    QVariant channelid = ch_model->data(ch_index,Roles::IdRole);
    (*device).setSingleProperty("ChannelID",channelid);
    mDb.deviceDao.updateDevice(*device);
    ch_model->addDeviceToChannel(device,ch_index);
    endResetModel();
}

void DeviceModel::removeDeviceFromChannel(const QModelIndex& dev_index, ChannelModel* ch_model,  const QModelIndex& ch_index)
{
    beginResetModel();
    shared_ptr<Device>& device = mDevices->at(dev_index.row());
    (*device).setSingleProperty("ChannelID",-1);
    mDb.deviceDao.updateDevice(*device);
    ch_model->removeDeviceFromChannel(device,ch_index);
    endResetModel();
}

QModelIndex DeviceModel::index(int row, int column,
                  const QModelIndex &parent) const {

    if (!hasIndex(row, column, parent))
        return createIndex(row, column, mDevices->at(row).get());

    if (!parent.internalPointer()) {
        return createIndex(row, column, mDevices->at(row).get());
    }

    QObject* obj = static_cast<QObject*>(parent.internalPointer());

    if (obj->property("ObjType").toString() == "root")
    {
        return createIndex(row, column, mDevices->at(row).get());
    }
    else if (obj->property("ObjType").toString() == "device")
    {
        Device* dev = static_cast<Device*>(parent.internalPointer());
        // is device
        if (dev->mVariableList.size() >= row)
        {
            return createIndex(row, column, dev->mVariableList.at(row).get());
        }
        else {
            return QModelIndex();
        }
    }
    else if (obj->property("ObjType").toString() == "variable")
    {
        //Variable* var = static_cast<Variable*>((void*)dev);
        return QModelIndex();
    }
    else {
        return QModelIndex();
    }
}

int DeviceModel::rowCount(const QModelIndex& parent) const {

    if (!parent.isValid()) {
        return mDevices->size();
    }

    QObject* obj = static_cast<QObject*>(parent.internalPointer());

    if (obj->property("ObjType").toString() == "root")
    {
        return mDevices->size();
    }
    else if (obj->property("ObjType").toString() == "device") {
        // is device
        Device* dev = static_cast<Device*>(parent.internalPointer());
        return dev->mVariableList.size();
    }
    else if (obj->property("ObjType").toString() == "variable")
    {
        return 0;
    }
    else {
        return 0;
    }

}

QModelIndex DeviceModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QObject* obj = static_cast<QObject*>(index.internalPointer());
    if (obj->property("ObjType").toString() == "root")
    {
        return QModelIndex();
    }
    else if (obj->property("ObjType").toString() == "device") {
        // is device
        //Device* dev = static_cast<Device*>(index.internalPointer());
        return createIndex(index.row(),index.column(),(void*)this);

    } else if (obj->property("ObjType").toString() == "variable"){
        //var = static_cast<Variable*>((void*)dev);

        Variable* var = static_cast<Variable*>(index.internalPointer());
        QVariant var_id = var->getSingleProperty("id");

        foreach(auto& dev, (*mDevices)) {
            for(int i = 0; i < dev->mVariableList.size(); i++)
            {
                if (dev->mVariableList.at(i)->getSingleProperty("id") == var_id) {
                    return createIndex(i,index.column(),dev.get());
                }
            }
        }
        return QModelIndex();
    }
    else {
        return QModelIndex();
    }
}

int DeviceModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant DeviceModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return "Device List";

    return QVariant();
}


QVariant DeviceModel::data(const QModelIndex& index, int role) const {

    if (!index.isValid()) {
        return QVariant();
    }

    QObject* obj = static_cast<QObject*>(index.internalPointer());


    if (obj->property("ObjType").toString() == "root")
    {
        return QVariant();
    }
    else if (obj->property("ObjType").toString() == "device") {
        // is device
        Device* dev = static_cast<Device*>(index.internalPointer());
        switch (role) {
            case Qt::DisplayRole:
                return dev->m_properties["Name"].toString() + "-" + dev->m_properties["NodeID"].toString();
            case Roles::IdRole:
                return dev->m_properties[DeviceHeaderList[0]];
            default:
                return QVariant();
        }
    } else if (obj->property("ObjType").toString() == "variable"){
        Variable* var = static_cast<Variable*>(index.internalPointer());

        switch (role) {
            case Qt::DisplayRole:
                return var->m_properties["Name"];
            default:
                return QVariant();
        }
    }

}


bool DeviceModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!isIndexValid(index)
            || role != Roles::NameRole) {
        return false;
    }
    Device& device = *mDevices->at(index.row());
//    device.m_portName =  value.toString();
    mDb.deviceDao.updateDevice(device);
    emit dataChanged(index, index);
    return true;
}

bool DeviceModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while (countLeft--) {
        const Device& device = *mDevices->at(row + countLeft);
        mDb.deviceDao.removeDevice(device.m_id);
    }
    mDevices->erase(mDevices->begin() + row, mDevices->begin() + row + count);
    endRemoveRows();
    return true;

}


QHash<int, QByteArray> DeviceModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::NameRole] = "name";
    return roles;
}

bool DeviceModel::isIndexValid(const QModelIndex& index) const
{
    return index.isValid() && index.row() < rowCount();
}

DeviceProxyModel::DeviceProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool DeviceProxyModel::filterAcceptsRow(int sourceRow,
                                              const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 4, sourceParent);

    QVariant tmp = sourceModel()->data(index);

    return (tmp.toInt() == mChID);
}

void DeviceProxyModel::setChannelID(int id) {
    mChID = id;
}

QVariant DeviceProxyModel::data(const QModelIndex &index, int role) const noexcept
{

    QModelIndex in = sourceModel()->index(mapToSource(index).row(),1);

    switch (role) {
        case Qt::DisplayRole:
            return sourceModel()->data(in);
        default:
            return QVariant();
    }
}

