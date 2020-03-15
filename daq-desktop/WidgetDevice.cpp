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

    for (int i = 0; i < dev->mCommands.size(); i++) {
        WidgetCommand* wc = new WidgetCommand(dev,i,this);
        mWidgetCommandList.append(wc);
        ui->verticalLayout_3->setAlignment(Qt::AlignTop);
        ui->verticalLayout_3->addWidget(wc);
    }
}

WidgetDevice::~WidgetDevice()
{
    delete ui;
}
