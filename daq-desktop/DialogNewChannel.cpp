#include "DialogNewChannel.h"
#include "ui_DialogNewChannel.h"
#include <QDebug>
#include <QStringListModel>
#include "Channel/ChannelModel.h"

DialogNewChannel::DialogNewChannel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewChannel)
{
    ui->setupUi(this);
    getAvailablePorts();
}



DialogNewChannel::~DialogNewChannel()
{
    qDebug() << "new channel dialog destroyed";
    delete ui;
}

QHash<QString, QVariant> DialogNewChannel::getChannelInfo() {

    QHash<QString, QVariant> prop;

    prop[ChannelHeaderList[1]] = ui->cmbPort->currentText();
    prop[ChannelHeaderList[2]] = ui->cmbBaudRate->currentText().toInt();
    prop[ChannelHeaderList[3]] = ui->cmbDataBits->currentText().toInt();
    prop[ChannelHeaderList[4]] = ui->cmbParity->currentText();
    prop[ChannelHeaderList[5]] = ui->cmbStopBits->currentText().toInt();

    return prop;
}

void DialogNewChannel::getAvailablePorts() {

    m_portLists.clear();
    ui->cmbPort->clear();

    m_portLists = ChannelModel::getAvailablePorts();

    foreach(auto port, m_portLists) {
        ui->cmbPort->addItem(port);
    }


}
