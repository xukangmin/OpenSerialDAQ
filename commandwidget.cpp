#include "commandwidget.h"
#include "ui_commandwidget.h"

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
