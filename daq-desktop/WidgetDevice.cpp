#include "WidgetDevice.h"
#include "ui_WidgetDevice.h"

WidgetDevice::WidgetDevice(Device *dev, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDevice),  mDevID(dev->m_id), mDevice(dev)
{
    ui->setupUi(this);

    ui->leName->setText(dev->getSingleProperty("Name").toString());

    ui->sbNode->setValue(dev->getSingleProperty("NodeID").toInt());

    ui->leProtocol->setText(dev->getSingleProperty("Protocol").toString());
}

WidgetDevice::~WidgetDevice()
{
    delete ui;
}
