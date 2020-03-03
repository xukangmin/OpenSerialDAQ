#include "WidgetDevicePage.h"
#include "ui_WidgetDevicePage.h"

WidgetDevicePage::WidgetDevicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDevicePage)
{
    ui->setupUi(this);
}

void WidgetDevicePage::setModel(Models *models)
{
    m_ch_model = models->mChannelModel;
    m_dev_model = models->mDeviceModel;

    ui->deviceListView->setModel(models->mDeviceModel);
}

WidgetDevicePage::~WidgetDevicePage()
{
    delete ui;
}
