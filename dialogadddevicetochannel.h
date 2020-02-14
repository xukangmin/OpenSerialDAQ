#ifndef DIALOGADDDEVICETOCHANNEL_H
#define DIALOGADDDEVICETOCHANNEL_H

#include <QDialog>
#include "device.h"
#include "databasemanager.h"

namespace Ui {
class DialogAddDeviceToChannel;
}

class DialogAddDeviceToChannel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddDeviceToChannel(int channelID, QWidget *parent = nullptr);
    ~DialogAddDeviceToChannel();

    QVector<int> getSelectedDeviceID();
    int m_channel_id;

private:
    Ui::DialogAddDeviceToChannel *ui;
    QVector<Device*> m_device_list;
};

#endif // DIALOGADDDEVICETOCHANNEL_H
