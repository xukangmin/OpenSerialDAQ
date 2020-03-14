#include "WidgetVariable.h"
#include "ui_WidgetVariable.h"

WidgetVariable::WidgetVariable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetVariable)
{
    ui->setupUi(this);
}

WidgetVariable::~WidgetVariable()
{
    delete ui;
}
