#include "minidevicewidget.h"
#include "ui_minidevicewidget.h"

MiniDeviceWidget::MiniDeviceWidget(QString devName, int devID, QWidget *parent) :
    QWidget(parent),
    m_devName(devName),
    m_devID(devID),
    ui(new Ui::MiniDeviceWidget)
{
    ui->setupUi(this);

    ui->lbDeviceName->setText(devName);
}

MiniDeviceWidget::~MiniDeviceWidget()
{
    delete ui;
}

void MiniDeviceWidget::on_btnDelete_clicked()
{
    emit deleteDeviceFromChannel(m_devID);
}
