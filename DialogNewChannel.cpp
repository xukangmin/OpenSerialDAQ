#include "DialogNewChannel.h"
#include "ui_DialogNewChannel.h"
#include <QDebug>

DialogNewChannel::DialogNewChannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewChannel)
{
    ui->setupUi(this);
}



DialogNewChannel::~DialogNewChannel()
{
    qDebug() << "new channel dialog destroyed";
    delete ui;
}

Channel DialogNewChannel::getChannelInfo() {
    Channel ch(0,
                ui->cmbPort->currentText(),
               ui->cmbBaudRate->currentText().toInt(),
                ui->cmbDataBits->currentText().toInt(),
                ui->cmbParity->currentText(),
               ui->cmbStopBits->currentText().toInt());

    return ch;
}

void DialogNewChannel::getAvailablePorts() {

    m_portLists.clear();
    ui->cmbPort->clear();
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        m_portLists.append(port.portName());
        ui->cmbPort->addItem(port.portName());
    }


}
