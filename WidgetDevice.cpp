#include "WidgetDevice.h"
#include "ui_WidgetDevice.h"

DeviceWidget::DeviceWidget(Device *dev, QVector<Channel> ch_list, QWidget *parent) :
    QWidget(parent),
    m_dev(dev),
    m_ch_list(ch_list),
    ui(new Ui::DeviceWidget)
{
    ui->setupUi(this);

    ui->cmbChannel->clear();

    foreach(Channel ch, m_ch_list) {
        ui->cmbChannel->addItem(ch.m_portName);
    }

    ui->label->setText(dev->m_protocol);

}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}
