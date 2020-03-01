#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include "Device.h"
#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>
#include "Database/DatabaseManager.h"

class DAQCORESHARED_EXPORT DeviceModel : public QAbstractTableModel
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

    static QStringList getAvailableComPorts();
    bool isPortExists(QString portName);
    QModelIndex addDevice(Device& channel);
    QVector<Device*> getAllDevices();
    Device* getDevice(int id);
    void removeByID(int id);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;

private:
    bool isIndexValid(const QModelIndex& index) const;



private:
    DatabaseManager& mDb;
    std::unique_ptr<std::vector<std::unique_ptr<Device>>> mDevices;
    int getIndexFromID(int id);
};


#endif // DEVICEMODEL_H
