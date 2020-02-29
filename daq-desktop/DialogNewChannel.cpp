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

    QHash<QString, QVariant> prop;

    prop[channelHeaderList[0]] = 0;
    prop[channelHeaderList[1]] = ui->cmbPort->currentText();
    prop[channelHeaderList[2]] = ui->cmbBaudRate->currentText().toInt();
    prop[channelHeaderList[3]] = ui->cmbDataBits->currentText().toInt();
    prop[channelHeaderList[4]] = ui->cmbParity->currentText();
    prop[channelHeaderList[5]] = ui->cmbStopBits->currentText().toInt();

    Channel ch(0,prop);

    return ch;
}

void DialogNewChannel::getAvailablePorts() {

    m_portLists.clear();
    ui->cmbPort->clear();

    m_portLists = ThreadChannel::getAvailablePorts();

    foreach(auto port, m_portLists) {
        ui->cmbPort->addItem(port);
    }


}
