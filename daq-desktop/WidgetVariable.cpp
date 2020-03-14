#include "WidgetVariable.h"
#include "ui_WidgetVariable.h"

WidgetVariable::WidgetVariable(Variable* var, QWidget *parent) :
    QWidget(parent), m_var_id(var->m_id), mVariable(var),
    ui(new Ui::WidgetVariable)
{
    ui->setupUi(this);

    ui->verticalLayout_2->setAlignment(Qt::AlignTop);

    ui->leName->setText(var->getSingleProperty("Name").toString());

    ui->leUnit->setText(var->getSingleProperty("Unit").toString());

    ui->leCurrentValue->setText(var->getSingleProperty("CurrentValue").toString());
}

WidgetVariable::~WidgetVariable()
{
    delete ui;
}
