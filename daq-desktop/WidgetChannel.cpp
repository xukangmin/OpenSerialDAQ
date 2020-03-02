#include "WidgetChannel.h"
#include "ui_WidgetChannel.h"
#include <QDebug>

ChannelWidget::ChannelWidget(ChannelModel* model, int row_index , QWidget *parent) :
    QWidget(parent), m_model(model), m_row_index(row_index),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    ui->ChannelInfo->setText(model->data(model->index(row_index, 1)).toString());

}



ChannelWidget::~ChannelWidget()
{
    qDebug() << "channel widget destroyed";
    delete ui;
}

void ChannelWidget::on_btnDelete_clicked()
{
    m_model->removeRows(m_row_index,1);
    //deleteLater();
}


void ChannelWidget::on_btnStart_clicked()
{
    m_model->startChannel(m_model->index(m_row_index,0));
}

void ChannelWidget::on_btnStop_clicked()
{
    m_model->stopChannel(m_model->index(m_row_index,0));
}

void ChannelWidget::on_btnAddDevice_clicked()
{
    //emit addDeviceToChannel(m_ch.m_id);
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
