#include "WidgetChannel.h"
#include "WidgetMiniDevice.h"
#include "ui_WidgetChannel.h"
#include <QDebug>

ChannelWidget::ChannelWidget(Models* models, QWidget *parent) :
    QWidget(parent), m_ch_model(models->mChannelModel), m_row_index(models->mChannelIndex.row()), m_dev_model(models->mDeviceModel),
    mModels(models),
    ui(new Ui::ChannelWidget)
{
    ui->setupUi(this);

    ui->ChannelInfo->setText(m_ch_model->data(m_ch_model->index(m_row_index, 1)).toString());

    for(int i = 0; i < m_dev_model->rowCount(); i++) {
        QModelIndex in = m_dev_model->index(i,4);
        QModelIndex inch = models->mChannelModel->index(m_row_index,0);
        if (m_dev_model->data(in).toInt() == m_ch_model->data(inch).toInt()) {  // channel id = id
            addDeviceToChannel(in);
        }
    }


}



ChannelWidget::~ChannelWidget()
{
    qDebug() << "channel widget destroyed";
    delete ui;
}

void ChannelWidget::addDeviceToChannel(const QModelIndex& dev_index) {
    mModels->mDeviceIndex = mModels->mDeviceModel->index(dev_index.row(),1); // make sure its using name
    MiniDeviceWidget *miniDeviceWidget = new MiniDeviceWidget(mModels,this);
    m_dev_model->addDeviceToChannel(dev_index, m_ch_model, m_ch_model->index(m_row_index,0));
    ui->verticalLayout->addWidget(miniDeviceWidget);
}

void ChannelWidget::on_btnDelete_clicked()
{
    for(int i = 0; i < m_dev_model->rowCount(); i++) {
        QModelIndex in = m_dev_model->index(i,4);
        QModelIndex inch = m_ch_model->index(m_row_index,0);
        if (m_dev_model->data(in).toInt() == m_ch_model->data(inch).toInt()) {  // channel id = id
            mModels->mDeviceIndex = in;
            m_dev_model->removeDeviceFromChannel(in,m_ch_model,inch);
        }
    }

    m_ch_model->removeRows(m_row_index,1);
    //deleteLater();
}


void ChannelWidget::on_btnStart_clicked()
{
    m_ch_model->startChannel(m_ch_model->index(m_row_index,0));
}

void ChannelWidget::on_btnStop_clicked()
{
    m_ch_model->stopChannel(m_ch_model->index(m_row_index,0));
}

void ChannelWidget::on_btnAddDevice_clicked()
{
    mDialogAddDevice = new DialogAddDeviceToChannel(m_dev_model,this);

    if (mDialogAddDevice->exec() == QDialog::Accepted) {

         QList<QModelIndex> deviceList = mDialogAddDevice->getSelectedDevice();

         foreach(auto index, deviceList) {
            addDeviceToChannel(index);
         }
    }

    mDialogAddDevice->deleteLater();
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
