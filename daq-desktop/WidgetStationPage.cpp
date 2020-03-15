#include "WidgetStationPage.h"
#include "ui_WidgetStationPage.h"

WidgetStationPage::WidgetStationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStationPage)
{
    ui->setupUi(this);
}

WidgetStationPage::~WidgetStationPage()
{
    delete ui;
}
