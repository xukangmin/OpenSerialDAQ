#include "MainWindow.h"
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
    DatabaseManager::instance().getAllDevice(&m_dev_list, &m_var_list);

    if (m_dev_list.empty()) {
        // insert demo device
        DatabaseManager::instance().insertDevice("ALICAT",0,"LFE-ALICAT");
        DatabaseManager::instance().insertDevice("DP",0,"LFE-DP");
        DatabaseManager::instance().insertDevice("DP",1,"LFE-DP");
        DatabaseManager::instance().getAllDevice(&m_dev_list, &m_var_list);
    }



    // register meta types
    qRegisterMetaType<QVector<DeviceData>>("QVector<DeviceData>");


    // Tool bar actions
    connect(ui->actionOverView,SIGNAL(triggered(bool)),this,SLOT(showOverViewPage()));
    connect(ui->actionDevices,SIGNAL(triggered(bool)),this,SLOT(showDevicePage()));


    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionAddNewChannel);


    // UI dialogs

    // Add new Channel UI
    connect(ui->actionAddNewChannel, SIGNAL(triggered()), this, SLOT(showNewChannelDialog()));




    m_channle_list = DatabaseManager::instance().getAllChannel();

    foreach(Channel ch, m_channle_list){
        createNewChannel(ch);
    }

    scrollVLayout = new QVBoxLayout;

    foreach(Device *dev, m_dev_list) {

        QListWidgetItem* lineItem = new QListWidgetItem(dev->m_name, ui->DeviceList);

        lineItem->setData(Qt::UserRole, dev->m_device_id);

        ui->DeviceList->addItem(lineItem);

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

    foreach(ThreadChannel* ch, m_Channels) {
        ch->stopChannel();
        ch->wait();
    }

    delete ui;
}

void MainWindow::showDevicePage() {
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::showOverViewPage() {
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::createNewChannel(Channel ch_info) {

    bool isExist = true;

    if (ch_info.m_id == 0)
    {
        isExist = false;
        ch_info.m_id = DatabaseManager::instance().insertChannel(ch_info);
    }

    ThreadChannel* ch = new ThreadChannel(ch_info,this);

    ChannelWidget* cw = new ChannelWidget(ch_info,this);

    connect(cw,&ChannelWidget::deleteChannel,this,&MainWindow::deleteChannel);

    connect(cw,&ChannelWidget::startChannel,ch,&ThreadChannel::startChannel);

    connect(cw,&ChannelWidget::stopChannel,ch,&ThreadChannel::stopChannel);

    connect(cw,&ChannelWidget::addDeviceToChannel,this,&MainWindow::showAddDeviceToChannelDialog);


    if (isExist) {
        QVector<Device*> tmpDevList;
        DatabaseManager::instance().getDeviceWithChannelID(ch_info.m_id, &tmpDevList);



//        foreach(const Device& dev, tmpDevList) {
//            MiniDeviceWidget* mw = new MiniDeviceWidget(dev.m_name,dev.m_device_id,cw);
//            connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,cw,&ChannelWidget::removeMiniDeviceWidget);
//            connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,ch,&ThreadChannel::removeDeviceFromChannel);
//            cw->addMiniDeviceWidget(mw);
//            ch->addDevice(dev);
//        }
    }

    ui->ChannelLayout->addWidget(cw);

    m_ChannelWidgets.append(cw);
    m_Channels.append(ch);

    qDebug() << "create new channel";
}

void MainWindow::showNewChannelDialog() {
    m_dlgNewChennel = new DialogNewChannel(this);
    m_dlgNewChennel->getAvailablePorts();
    if (m_dlgNewChennel->exec() == QDialog::Accepted) {

        Channel chinfo(m_dlgNewChennel->getChannelInfo());

        bool exists = false;
        foreach(ThreadChannel* ch, m_Channels) {
            if (ch->m_ch.m_portName == chinfo.m_portName) {
                exists = true;
            }
        }

        if (!exists) {
            createNewChannel(chinfo);
        } else {
            QMessageBox msgBox;
            msgBox.setText("Same Com port already exists!");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }



    }

    m_dlgNewChennel->deleteLater();
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

                    foreach(ThreadChannel* sg, m_Channels) {
                        if (sg->m_ch_id == id) {
//                            foreach(Device *dev, m_dev_list) {
//                                if (dev->m_device_id == dev_id) {
//                                    sg->addDevice(dev);
//                                    connect(mw,&MiniDeviceWidget::deleteDeviceFromChannel,sg,&ThreadChannel::removeDeviceFromChannel);
//                                }
//                            }
                        }
                    }
                }
            }
        }
    }

    m_dlgAddDeviceToChannel->deleteLater();

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
            ui->ChannelLayout->removeWidget(m_ChannelWidgets[i]);
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
