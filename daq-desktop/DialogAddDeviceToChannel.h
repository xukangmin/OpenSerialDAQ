#ifndef DIALOGADDDEVICETOCHANNEL_H
#define DIALOGADDDEVICETOCHANNEL_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "Device/DeviceModel.h"

namespace Ui {
class DialogAddDeviceToChannel;
}

class DialogAddDeviceToChannel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddDeviceToChannel(DeviceModel *dev_model, QWidget *parent = nullptr);
    ~DialogAddDeviceToChannel();

    QList<QModelIndex> getSelectedDevice();
    int m_channel_id;

private:
    Ui::DialogAddDeviceToChannel *ui;
    DeviceProxyModel *proxyModel;
};

#endif // DIALOGADDDEVICETOCHANNEL_H
