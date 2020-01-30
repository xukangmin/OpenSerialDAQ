#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOverView,SIGNAL(triggered(bool)),this,SLOT(showOverViewPage()));
    connect(ui->actionDevices,SIGNAL(triggered(bool)),this,SLOT(showDevicePage()));


    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);



    qRegisterMetaType<QVector<DeviceData>>("QVector<DeviceData>");

    DatabaseManager::instance().init();

    m_dev_list = DatabaseManager::instance().getAllDevice();

    while (m_dev_list.empty()) {
        // insert demo device
        DatabaseManager::instance().insertDevice(0,"LFE-ALICAT");
        DatabaseManager::instance().insertDevice(0,"LFE-DP");
        m_dev_list = DatabaseManager::instance().getAllDevice();
    }



    m_singleChannel = new SingleChannel(this);

    connect(m_singleChannel, &SingleChannel::sendData, this, &MainWindow::getData);

    foreach(Device* dev, m_dev_list) {
        m_singleChannel->addDevice(dev);
    }

    m_singleChannel->startChannel();

    m_singleChannel->startDAQ();
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
    m_singleChannel->stopChannel();
    m_singleChannel->wait();
    delete ui;
}

void MainWindow::showDevicePage() {
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::showOverViewPage() {
    ui->stackedWidget_2->setCurrentIndex(0);
}
