#include "dialogadddevicetochannel.h"
#include "ui_dialogadddevicetochannel.h"

#include <QStringListModel>
#include <QListWidgetItem>

DialogAddDeviceToChannel::DialogAddDeviceToChannel(int channelID, QWidget *parent) :
    QDialog(parent),
    m_channel_id(channelID),
    ui(new Ui::DialogAddDeviceToChannel)
{
    ui->setupUi(this);

    // first get device are not assigned to any channel
    m_device_list = DatabaseManager::instance().getUnassignedDevice();


    foreach(Device* dev, m_device_list) {

        QListWidgetItem* lineItem = new QListWidgetItem(dev->m_name, ui->lsDevice);

        lineItem->setData(Qt::UserRole, dev->m_device_id);

        ui->lsDevice->addItem(lineItem);
    }


}


QVector<int> DialogAddDeviceToChannel::getSelectedDeviceID()
{
    QVector<int> ls;

    foreach(QListWidgetItem* lineItem, ui->lsDevice->selectedItems()) {
        //ls.append(lineItem->data(Qt::UserRole));
        ls.append(lineItem->data(Qt::UserRole).toInt());
    }

    return ls;
}



DialogAddDeviceToChannel::~DialogAddDeviceToChannel()
{
    delete ui;
}
