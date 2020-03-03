#include "DialogAddDeviceToChannel.h"
#include "ui_DialogAddDeviceToChannel.h"

#include <QStringListModel>
#include <QListWidgetItem>

DialogAddDeviceToChannel::DialogAddDeviceToChannel(DeviceModel *dev_model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddDeviceToChannel)
{
    ui->setupUi(this);

    proxyModel = new DeviceProxyModel(this);

    proxyModel->setChannelID(-1);
    proxyModel->setSourceModel(dev_model);

    ui->listView->setModel(proxyModel);
    ui->listView->setSelectionMode(QAbstractItemView::MultiSelection);
    // first get device are not assigned to any channel
//    m_device_list = DatabaseManager::instance().getUnassignedDevice();


//    foreach(Device* dev, m_device_list) {

//        QListWidgetItem* lineItem = new QListWidgetItem(dev->m_name, ui->lsDevice);

//        lineItem->setData(Qt::UserRole, dev->m_device_id);

//        ui->lsDevice->addItem(lineItem);
//    }


}


QList<QModelIndex> DialogAddDeviceToChannel::getSelectedDevice()
{
    QList<QModelIndex> ls;

    QItemSelection selectionProxy = ui->listView->selectionModel()->selection();

    QItemSelection selectionSource = proxyModel->mapSelectionToSource(selectionProxy);

    ls = selectionSource.indexes();

//    foreach(QListWidgetItem* lineItem, ui->lsDevice->selectedItems()) {
//        //ls.append(lineItem->data(Qt::UserRole));
//        ls.append(lineItem->data(Qt::UserRole).toInt());
//    }

    return ls;
}



DialogAddDeviceToChannel::~DialogAddDeviceToChannel()
{
    delete ui;
}
