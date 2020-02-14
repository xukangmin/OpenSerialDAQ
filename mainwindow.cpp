#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMetaType>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init DB
    DatabaseManager::instance().init();

    // get all device from db
    m_dev_list = DatabaseManager::instance().getAllDevice();

    while (m_dev_list.empty()) {
        // insert demo device
        DatabaseManager::instance().insertDevice("ALICAT",0,"LFE-ALICAT");
        DatabaseManager::instance().insertDevice("DP",0,"LFE-DP");
        DatabaseManager::instance().insertDevice("DP",1,"LFE-DP");
        m_dev_list = DatabaseManager::instance().getAllDevice();
    }


    qRegisterMetaType<QVector<DeviceData>>("QVector<DeviceData>");

    ui->conChannelOut->setLayout(ui->conChannel);

    connect(ui->actionOverView,SIGNAL(triggered(bool)),this,SLOT(showOverViewPage()));
    connect(ui->actionDevices,SIGNAL(triggered(bool)),this,SLOT(showDevicePage()));


    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionAddNewChannel);

    // UI dialogs

    m_dlgNewChennel = new NewChannel(this);


    connect(ui->actionAddNewChannel, SIGNAL(triggered()), this, SLOT(showNewChannelDialog()));

    m_channle_list = DatabaseManager::instance().getAllChannel();

    foreach(Channel ch, m_channle_list){
        SingleChannel* ch_thread = new SingleChannel(ch,this);
        ChannelWidget* cw = new ChannelWidget(ch, this);

        connect(cw,&ChannelWidget::deleteChannel,this,&MainWindow::deleteChannel);

        connect(cw,&ChannelWidget::startChannel,ch_thread,&SingleChannel::startChannel);

        connect(cw,&ChannelWidget::stopChannel,ch_thread,&SingleChannel::stopChannel);

        connect(cw,&ChannelWidget::addDeviceToChannel,this,&MainWindow::showAddDeviceToChannelDialog);

        QVector<Device*> tmpDevList = DatabaseManager::instance().getDeviceWithChannelID(ch.m_id);



        foreach(Device* dev, tmpDevList) {
            MiniDeviceWidget* mw = new MiniDeviceWidget(dev->m_name,dev->m_device_id,cw);
            connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,cw,&ChannelWidget::removeMiniDeviceWidget);
            connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,ch_thread,&SingleChannel::removeDeviceFromChannel);
            cw->addMiniDeviceWidget(mw);
            ch_thread->addDevice(dev);
        }

        ui->conChannel->addWidget(cw);
        m_Channels.append(ch_thread);
        m_ChannelWidgets.append(cw);
    }

    scrollVLayout = new QVBoxLayout;

    foreach(Device* dev, m_dev_list) {

        QListWidgetItem* lineItem = new QListWidgetItem(dev->m_name, ui->listDevice);

        lineItem->setData(Qt::UserRole, dev->m_device_id);

        ui->listDevice->addItem(lineItem);

        //connect(ui->listDevice,SIGNAL(clicked(const QModelIndex & index)),this,SLOT(deviceSelected(QModelIndex index)));
        DeviceWidget* dw = new DeviceWidget(dev, m_channle_list, this);
        dw->hide();
        m_DeviceWidgets.append(dw);

    }

    //ui->scrollAreaWidgetContents->setLayout(scrollVLayout);


//    DatabaseManager::instance().init();




//    //m_singleChannel = new SingleChannel(this);

//    connect(m_singleChannel, &SingleChannel::sendData, this, &MainWindow::getData);



//    m_singleChannel->startChannel();

//    m_singleChannel->startDAQ();
}

void MainWindow::getData(QVector<DeviceData> data, int ch_id){

    qDebug() << data.size();
    if (data.size() == 4)
    {
        qDebug() << data[0].m_currentData;
        //ui->lineEdit->setText(QString::number(data[0].m_currentData));
    }

    qDebug() << "get data from main window" << ch_id;
}

MainWindow::~MainWindow()
{
    //m_singleChannel->stopChannel();
    //m_singleChannel->wait();

    foreach(SingleChannel* ch, m_Channels) {
        ch->stopChannel();
        ch->wait();
    }

    delete ui;
}

void MainWindow::showDevicePage() {
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::showOverViewPage() {
    ui->stackedWidget_2->setCurrentIndex(0);
}




void MainWindow::showNewChannelDialog() {
    m_dlgNewChennel->getAvailablePorts();
    if (m_dlgNewChennel->exec() == QDialog::Accepted) {

        Channel chinfo(m_dlgNewChennel->getChannelInfo());

        bool exists = false;
        foreach(SingleChannel* ch, m_Channels) {
            if (ch->m_ch.m_portName == chinfo.m_portName) {
                exists = true;
            }
        }

        if (!exists) {
            int id = DatabaseManager::instance().insertChannel(chinfo);

            chinfo.m_id = id;

            SingleChannel* ch = new SingleChannel(chinfo,this);

            ChannelWidget* cw = new ChannelWidget(chinfo,this);

            connect(cw,&ChannelWidget::deleteChannel,this,&MainWindow::deleteChannel);

            connect(cw,&ChannelWidget::startChannel,ch,&SingleChannel::startChannel);

            ui->conChannel->addWidget(cw);

            m_ChannelWidgets.append(cw);
            m_Channels.append(ch);

            qDebug() << "create new channel";
        } else {
            QMessageBox msgBox;
            msgBox.setText("Same Com port already exists!");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }



    }
}

void MainWindow::showAddDeviceToChannelDialog(int id) {

     m_dlgAddDeviceToChannel = new DialogAddDeviceToChannel(id, this);
    if (m_dlgAddDeviceToChannel->exec() == QDialog::Accepted) {
        QVector<int> selectedDevID = m_dlgAddDeviceToChannel->getSelectedDeviceID();

        foreach(int dev_id, selectedDevID) {
            DatabaseManager::instance().updateDeviceWithChannelID(dev_id, id);

            foreach(ChannelWidget* wg, m_ChannelWidgets) {
                if (wg->getChannelID() == id) {
                    MiniDeviceWidget* mw = new MiniDeviceWidget(QString::number(dev_id), dev_id, wg);
                    wg->addMiniDeviceWidget(mw);
                    connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,wg,&ChannelWidget::removeMiniDeviceWidget);

                    foreach(SingleChannel* sg, m_Channels) {
                        if (sg->m_ch_id == id) {
                            foreach(Device *dev, m_dev_list) {
                                if (dev->m_device_id == dev_id) {
                                    sg->addDevice(dev);
                                    connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,sg,&SingleChannel::removeDeviceFromChannel);
                                }
                            }
                        }
                    }
                }
            }



//            foreach(SingleChannel* sc, m_Channels) {
//                if (sc->m_ch_id
//            }
        }
    }

}

void MainWindow::deleteChannel(int id) {
    for (int i = 0; i < m_Channels.size(); i++) {
        if (m_Channels[i]->m_ch_id == id)
        {
            m_Channels[i]->stopDAQ();
            m_Channels[i]->stopChannel();
            m_Channels[i]->wait();
            m_Channels[i]->deleteLater();
            m_Channels.remove(i);
        }
    }


    for (int i = 0; i < m_ChannelWidgets.size(); i++) {
        if (m_ChannelWidgets[i]->m_ch.m_id == id)
        {
            ui->conChannel->removeWidget(m_ChannelWidgets[i]);
            m_ChannelWidgets[i]->deleteLater();
            m_ChannelWidgets.remove(i);
        }
    }

       DatabaseManager::instance().removeChannel(id);
}

//void MainWindow::deviceSelected(QModelIndex index) {
//    qDebug() << index.row();
//}

//void MainWindow::on_listDevice_clicked(const QModelIndex &index)
//{
//    int dev_id = index.data(Qt::UserRole).toInt();

//    bool found = false;


//    foreach(DeviceWidget* dw, m_DeviceWidgets) {
//        ui->layDevice->removeWidget(dw);
//        if (dw->m_dev->m_device_id == dev_id) {
//            found = true;
//            dw->show();
//            ui->scrDevice->setLayout(ui->layDevice);
//            ui->layDevice->addWidget(dw);
//        }
//    }

//}
