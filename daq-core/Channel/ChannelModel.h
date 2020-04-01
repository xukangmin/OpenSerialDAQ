#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>

#include "Channel.h"
#include "Database/DatabaseManager.h"

class DAQCORESHARED_EXPORT ChannelModel : public QAbstractTableModel
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
    ChannelModel(QObject* parent = nullptr);

    static QStringList getAvailableComPorts();
    bool isPortExists(QString portName);
    QModelIndex addChannel(QHash<QString,QVariant> properties);

    static QList<QString> getAvailablePorts();
//    QVector<Channel*> getAllChannels();
//    Channel* getChannel(int id);
//    void removeByID(int id);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;

    void startChannel(const QModelIndex& index);
    void stopChannel(const QModelIndex& index);
    bool findChannelByName(QString name, std::shared_ptr<Channel>& ch);
    void addDeviceToChannel(const std::shared_ptr<Device>& dev, const QModelIndex& ch_index);
    void addDeviceToChannel(const std::shared_ptr<Device>& dev, const std::shared_ptr<Channel>& ch);
    void removeDeviceFromChannel(const std::shared_ptr<Device>& dev, const QModelIndex& ch_index);
    void removeAllRows(const QModelIndex& parent = QModelIndex());
private:
    bool isIndexValid(const QModelIndex& index) const;



private:
    DatabaseManager& mDb;
    std::unique_ptr<std::vector<std::shared_ptr<Channel>>> mChannels;
};

#endif // CHANNELMODEL_H
