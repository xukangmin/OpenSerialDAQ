#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<currentData>("currentData");
    m_db.init();

    m_dev_list = m_db.getAllDevice();

    while (m_dev_list.empty()) {
        // insert demo device
        m_db.insertDevice(0,"LFE-ALICAT");
        m_dev_list = m_db.getAllDevice();
    }



    m_singleChannel = new SingleChannel(this);

    connect(m_singleChannel, &SingleChannel::sendData, this, &MainWindow::getData);

    foreach(Device* dev, m_dev_list) {
        m_singleChannel->addDevice(dev);
    }

    m_singleChannel->startChannel();

    m_singleChannel->startDAQ();
}

void MainWindow::getData(currentData data, int ch_id){

    qDebug() << data.values.size();
    if (data.values.size() == 4)
    {
        qDebug() << data.values[0];
        qDebug() << data.values[1];
        ui->lineEdit->text() = data.values[3];
    }

    qDebug() << "get data from main window" << ch_id;
}

MainWindow::~MainWindow()
{
    m_singleChannel->stopChannel();
    m_singleChannel->wait();
    delete ui;
}

