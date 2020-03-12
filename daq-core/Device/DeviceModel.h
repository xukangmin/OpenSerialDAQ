#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include "Device.h"
#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>
#include <QSortFilterProxyModel>
#include "Database/DatabaseManager.h"
#include <Channel/ChannelModel.h>
#include "Variable/Variable.h"

class DAQCORESHARED_EXPORT DeviceModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum Roles {
           IdRole = Qt::UserRole + 1,
           NameRole,
           TypeRole,
           ComPortRole,
           BaudRateRole,
           ParityRole,
           DataBitsRole,
           StopBitsRole
    };
    DeviceModel(QObject* parent = nullptr);

    QModelIndex addDevice(QHash<QString,QVariant> properties);
    bool isDeviceExists(QString name, int node);
    void addDeviceToChannel(const QModelIndex& dev_index, ChannelModel* ch_model,  const QModelIndex& ch_index);
    void removeDeviceFromChannel(const QModelIndex& dev_index, ChannelModel* ch_model,  const QModelIndex& ch_index);
    int getDeviceIDByNameAndNode(QString name, int node);
    void addVariableToDevice(const std::shared_ptr<Variable>& var, const QModelIndex& dev_index);
    std::vector<QHash<QString, QVariant>> getVariableDefinitionFromDevice(const QModelIndex& dev_index);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QModelIndex parent(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;
    QList<QString> getAvailableProtocols();

private:
    bool isIndexValid(const QModelIndex& index) const;



private:
    DatabaseManager& mDb;
    std::unique_ptr<std::vector<std::shared_ptr<Device>>> mDevices;
    int getIndexFromID(int id);
};

class DAQCORESHARED_EXPORT DeviceProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DeviceProxyModel(QObject *parent = nullptr);

    void setChannelID(int id);

    int mChID;
    QVariant data(const QModelIndex &index, int role) const noexcept override;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

};

#endif // DEVICEMODEL_H
