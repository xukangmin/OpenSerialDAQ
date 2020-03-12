#include "DialogNewDevice.h"
#include "ui_DialogNewDevice.h"
#include "Models.h"

DialogNewDevice::DialogNewDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewDevice)
{
    ui->setupUi(this);

    QList<QString> proc_list = Models::instance().mDeviceModel->getAvailableProtocols();

    ui->cbProtocol->addItems(proc_list);
}



DialogNewDevice::~DialogNewDevice()
{
    delete ui;
}

QHash<QString, QVariant> DialogNewDevice::getDeviceInfo()
{
    QHash<QString, QVariant> prop;

    prop["Name"] = ui->leDeviceName->text();
    prop["NodeID"] = ui->sbNodeID->value();
    prop["Protocol"] = ui->cbProtocol->currentText();
    prop["ChannelID"] = -1;

    return prop;
}
