#include "newchannel.h"
#include "ui_newchannel.h"
#include <QDebug>

NewChannel::NewChannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewChannel)
{
    ui->setupUi(this);
}



NewChannel::~NewChannel()
{
    delete ui;
}

Channel NewChannel::getChannelInfo() {
    Channel ch(0,
                ui->cmbPort->currentText(),
               ui->cmbBaudRate->currentText().toInt(),
                ui->cmbDataBits->currentText().toInt(),
                ui->cmbParity->currentText(),
               ui->cmbStopBits->currentText().toInt());

    return ch;
}

void NewChannel::getAvailablePorts() {

    m_portLists.clear();
    ui->cmbPort->clear();
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        m_portLists.append(port.portName());
        ui->cmbPort->addItem(port.portName());
    }


}
