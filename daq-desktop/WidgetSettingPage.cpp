#include "WidgetSettingPage.h"
#include "ui_WidgetSettingPage.h"

WidgetSettingPage::WidgetSettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSettingPage)
{
    ui->setupUi(this);
}

WidgetSettingPage::~WidgetSettingPage()
{
    delete ui;
}
