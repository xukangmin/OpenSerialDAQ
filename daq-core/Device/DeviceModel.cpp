#include "DeviceModel.h"
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
    root = new QStandardItem("Root");
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
    device->m_var_list.push_back(var);
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
        return QModelIndex();

    GenericDefinition* genericItem;
    Device* deviceItem;
    Variable* variableItem;

    if (!parent.isValid()) {
        return createIndex(row, column);
    }

    if (parent.internalPointer() == this) {
        // root item, get pointer to child of row
        return createIndex(row, column, this->mDevices->at(row).get());
    }

    genericItem = static_cast<GenericDefinition*>(parent.internalPointer());

    if (genericItem->m_properties.keys().contains("Procotol")) {
        // Device
        deviceItem = static_cast<Device*>(genericItem);

        if (deviceItem->m_var_list.size() >= row)
        {
            return createIndex(row, column, deviceItem->m_var_list.at(row).get());
        }
        else {
            return QModelIndex();
        }

    }
    else if (genericItem->m_properties.keys().contains("Equation"))
    {
        // variable
        variableItem = static_cast<Variable*>(genericItem);
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

    qDebug() << "parent.internalPointer()=" << QString::number((long)parent.internalPointer(),16);

    qDebug() << "this=" << QString::number((long)this,16);

    if (!parent.internalPointer()) {
        return 0;
    }

    if (!parent.isValid()) {
        // root item
        return mDevices->size();
    }

    GenericDefinition* genericItem;
    Device* deviceItem;
    //Variable* variableItem;

    genericItem = static_cast<GenericDefinition*>(parent.internalPointer());


    if (genericItem->m_properties.keys().contains("Procotol")) {
        // Device
        deviceItem = static_cast<Device*>(genericItem);
        return deviceItem->m_var_list.size();
    }
    else if (genericItem->m_properties.keys().contains("Equation"))
    {
        // variable
        return 0;
    } else {
        return 0;
    }


}

QModelIndex DeviceModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    GenericDefinition* genericItem;
    Variable* variableItem;
    Device* deviceItem;

    if (index.internalPointer() == this) {
        // root item
        return QModelIndex();
    }

    genericItem = static_cast<GenericDefinition*>(index.internalPointer());

    if (genericItem->m_properties.keys().contains("Procotol")) {
        // Device
        deviceItem = static_cast<Device*>(genericItem);

        QVariant devid_id = deviceItem->getSingleProperty("id");

        for(auto i = 0; i < (*mDevices).size(); i++) {
            if ((*mDevices).at(i)->getSingleProperty("id") == devid_id) {
                return createIndex(i, 0, (void*)this);
            }
        }
        return QModelIndex();
    }
    else if (genericItem->m_properties.keys().contains("Equation"))
    {
        // variable
        variableItem = static_cast<Variable*>(genericItem);
        QVariant var_id = variableItem->getSingleProperty("id");

        foreach(auto& dev, (*mDevices)) {
            for(int i = 0; i < dev->m_var_list.size(); i++)
            {
                if (dev->m_var_list.at(i)->getSingleProperty("id") == var_id) {
                    return createIndex(i,0,dev.get());
                }
            }
        }
        return QModelIndex();
    }
}

int DeviceModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return DeviceColumnSize;
}




QVariant DeviceModel::data(const QModelIndex& index, int role) const {

    if (!isIndexValid(index)) {
        return QVariant();
    }

    if (index.internalPointer() == this) {
        // root item
        return QVariant();
    }

    GenericDefinition* genericItem;
    Variable* variableItem;
    Device* deviceItem;

    genericItem = static_cast<GenericDefinition*>(index.internalPointer());

    if (!genericItem) {
        // Device
        const Device& device = *mDevices->at(index.row());


        switch (role) {
            case Qt::DisplayRole:
                return device.m_properties[DeviceHeaderList[0]];
            case Roles::IdRole:
                return device.m_properties[DeviceHeaderList[0]];
            default:
                return QVariant();
        }
    }

    if (genericItem->m_properties.keys().contains("Procotol")) {
        // Device
        const Device& device = *mDevices->at(index.row());


        switch (role) {
            case Qt::DisplayRole:
                return device.m_properties[DeviceHeaderList[index.column()]];
            case Roles::IdRole:
                return device.m_properties[DeviceHeaderList[0]];
            default:
                return QVariant();
        }
    }
    else if (genericItem->m_properties.keys().contains("Equation"))
    {
        // variable
        variableItem = static_cast<Variable*>(genericItem);
        QVariant dev_id = variableItem->getSingleProperty("DeviceID");

        for(auto i = 0; i < (*mDevices).size(); i++) {
            if ((*mDevices).at(i)->getSingleProperty("id") == dev_id) {
                deviceItem = (*mDevices).at(i).get();
            }
        }

        switch (role) {
            case Qt::DisplayRole:
                return variableItem->m_properties[VariableHeaderList[0]];
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

