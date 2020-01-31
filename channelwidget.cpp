#include "channelwidget.h"
#include "ui_channelwidget.h"
#include <QDebug>

ChannelWidget::ChannelWidget(Channel ch, QWidget *parent) :
    QWidget(parent),m_ch(ch),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    qDebug() << ch.m_portName;

    qDebug() << ch.m_baudRate;

    qDebug() << ch.m_dataBits;

    qDebug() << ch.m_parity;

    ui->ChannelInfo->setText(ch.m_portName + " " + ch.m_baudRateStr + " " + ch.m_dataBitsStr + "-" + ch.m_parityStr + "-" + ch.m_stopBitsStr);

}

ChannelWidget::~ChannelWidget()
{
    qDebug() << "channel widget " << m_ch.m_id << " destroyed";
    delete ui;
}

void ChannelWidget::on_btnDelete_clicked()
{
    emit deleteChannel(m_ch.m_id);
}


void ChannelWidget::on_btnStart_clicked()
{
    emit startChannel(m_ch.m_id);
}
