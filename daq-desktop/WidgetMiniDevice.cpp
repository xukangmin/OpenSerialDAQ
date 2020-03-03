#include "WidgetMiniDevice.h"
#include "ui_WidgetMiniDevice.h"

MiniDeviceWidget::MiniDeviceWidget(Models *models, QWidget *parent) :
    QWidget(parent),
    m_dev_model(models->mDeviceModel),
    m_dev_index(models->mDeviceIndex),
    mModels(models),
    ui(new Ui::MiniDeviceWidget)
{
    ui->setupUi(this);

    ui->lbDeviceName->setText(m_dev_model->data(m_dev_index).toString());
}

MiniDeviceWidget::~MiniDeviceWidget()
{
    delete ui;
}

void MiniDeviceWidget::on_btnDelete_clicked()
{
    m_dev_model->removeDeviceFromChannel(m_dev_index,mModels->mChannelModel,mModels->mChannelIndex);
    this->deleteLater();
    //emit deleteDeviceFromChannel(m_devID);
}
