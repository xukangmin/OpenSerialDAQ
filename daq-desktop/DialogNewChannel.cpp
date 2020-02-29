#include "DialogNewChannel.h"
#include "ui_DialogNewChannel.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QStringListModel>
#include "Channel/ChannelModel.h"

DialogNewChannel::DialogNewChannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewChannel)
{
    ui->setupUi(this);
    getAvailablePorts();
//    QStringListModel *model = new QStringListModel();
//    model->setStringList(ChannelModel::getAvailableComPorts());
//    ui->cmbPort->setModel(model);
}



DialogNewChannel::~DialogNewChannel()
{
    qDebug() << "new channel dialog destroyed";
    delete ui;
}

Channel DialogNewChannel::getChannelInfo() {

    QList<QVariant> prop;

    prop.append(0);
    prop.append(ui->cmbPort->currentText());
    prop.append(ui->cmbBaudRate->currentText().toInt());
    prop.append(ui->cmbDataBits->currentText().toInt());
    prop.append(ui->cmbParity->currentText());
    prop.append(ui->cmbStopBits->currentText().toInt());

    Channel ch(0,prop);

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
