#include "WidgetDevicePage.h"
#include "ui_WidgetDevicePage.h"

#include "Models.h"

WidgetDevicePage::WidgetDevicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDevicePage)
{
    ui->setupUi(this);

    ui->treeView->setModel(Models::instance().mDeviceModel);
}


WidgetDevicePage::~WidgetDevicePage()
{
    delete ui;
}
