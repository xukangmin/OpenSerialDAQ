#include "WidgetMiniVariable.h"
#include "ui_WidgetMiniVariable.h"

WidgetMiniVariable::WidgetMiniVariable(Variable* var, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMiniVariable), mVar(var)
{
    ui->setupUi(this);

    ui->leVarName->setText(var->getSingleProperty("Name").toString());
    ui->leEquation->setText(var->getSingleProperty("Equation").toString());

    ui->leValue->setText(var->getSingleProperty("CurrentValue").toString());

    ui->leTime->setText(var->getSingleProperty("CurrentTimeStamp").toString());
}

WidgetMiniVariable::~WidgetMiniVariable()
{
    delete ui;
}
