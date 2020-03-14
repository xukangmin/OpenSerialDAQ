#include "WidgetDevice.h"
#include "ui_WidgetDevice.h"

WidgetDevice::WidgetDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDevice)
{
    ui->setupUi(this);
}

WidgetDevice::~WidgetDevice()
{
    delete ui;
}
