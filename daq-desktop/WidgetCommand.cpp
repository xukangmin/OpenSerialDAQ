#include "WidgetCommand.h"
#include "ui_WidgetCommand.h"

WidgetCommand::WidgetCommand(Device* dev, int cmdIndex, QWidget *parent) :
    QGroupBox(parent), mCmd(dev->mCommands[cmdIndex]),
    ui(new Ui::WidgetCommand)
{
    ui->setupUi(this);

    ui->lblCmdName->setText(mCmd.name);

    ui->lblDescription->setText(mCmd.description);

}

WidgetCommand::~WidgetCommand()
{
    delete ui;
}
