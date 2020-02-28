#include "WidgetCommand.h"
#include "ui_WidgetCommand.h"

CommandWidget::CommandWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::CommandWidget)
{
    ui->setupUi(this);
}

CommandWidget::~CommandWidget()
{
    delete ui;
}
