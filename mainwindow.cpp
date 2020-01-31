#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMetaType>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DatabaseManager::instance().init();

    m_dev_list = DatabaseManager::instance().getAllDevice();

    while (m_dev_list.empty()) {
        // insert demo device
        DatabaseManager::instance().insertDevice("ALICAT",0,"LFE-ALICAT");
        DatabaseManager::instance().insertDevice("ALICAT",0,"LFE-DP");
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


    QVector<Channel> ch_list = DatabaseManager::instance().getAllChannel();

    foreach(Channel ch, ch_list){
        SingleChannel* ch_thread = new SingleChannel(ch,this);
        ChannelWidget* cw = new ChannelWidget(ch, this);

        connect(cw,&ChannelWidget::deleteChannel,this,&MainWindow::deleteChannel);

        connect(cw,&ChannelWidget::startChannel,ch_thread,&SingleChannel::startChannel);

        foreach(Device* dev, m_dev_list) {
            ch_thread->addDevice(dev);
        }

        ui->conChannel->addWidget(cw);
        m_Channels.append(ch_thread);
        m_ChannelWidgets.append(cw);
    }

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
