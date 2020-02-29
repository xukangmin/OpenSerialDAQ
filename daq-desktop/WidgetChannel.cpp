#include "WidgetChannel.h"
#include "ui_WidgetChannel.h"
#include <QDebug>

ChannelWidget::ChannelWidget(Channel& ch, QWidget *parent) :
    QWidget(parent),m_ch(ch),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    ui->ChannelInfo->setText(ch.getProperty("ComPort").toString());

}



ChannelWidget::~ChannelWidget()
{
    qDebug() << "channel widget " << m_ch.m_id << " destroyed";
    delete ui;
}

void ChannelWidget::on_btnDelete_clicked()
{
    emit deleteChannel(m_ch.m_id);
    deleteLater();
}


void ChannelWidget::on_btnStart_clicked()
{
    emit startChannel(m_ch.m_id);
}

void ChannelWidget::on_btnStop_clicked()
{
    emit stopChannel(m_ch.m_id);
}

void ChannelWidget::on_btnAddDevice_clicked()
{
    emit addDeviceToChannel(m_ch.m_id);
}

int ChannelWidget::getChannelID() {
    return m_ch.m_id;
}

//void ChannelWidget::addMiniDeviceWidget(MiniDeviceWidget* dev) {
//    ui->verticalLayout->addWidget(dev);
//}


//void ChannelWidget::removeMiniDeviceWidget(int dev_id) {

//        QList<MiniDeviceWidget *> mwg_list = this->findChildren<MiniDeviceWidget*>();

//        foreach(MiniDeviceWidget * mwg, mwg_list) {
//            if (mwg->m_devID == dev_id) {
//                ui->verticalLayout->removeWidget(mwg);
//                mwg->deleteLater();
//                //DatabaseManager::instance().resetDeviceBinding(dev_id);
//                // reset associated channel id
//            }
//        }
////    foreach(QWidget* wg, (MiniDeviceWidget*)ui->verticalLayout->widget()) {
////        MiniDeviceWidget* mwg = dynamic_cast<MiniDeviceWidget*>(wg);
////        if (mwg->m_devID == dev_id) {
////            ui->verticalLayout->removeWidget(mwg);
////        }
////    }
//}
