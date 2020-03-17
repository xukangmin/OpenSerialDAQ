#include "WidgetMiniVariable.h"
#include "ui_WidgetMiniVariable.h"

WidgetMiniVariable::WidgetMiniVariable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMiniVariable)
{
    ui->setupUi(this);
}

WidgetMiniVariable::~WidgetMiniVariable()
{
    delete ui;
}
