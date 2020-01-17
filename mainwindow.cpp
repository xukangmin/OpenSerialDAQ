#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "device.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Device* alicatA = new Device(0,"LFE-ALICAT");

    m_singleChannel.addDevice(alicatA);

    m_singleChannel.startChannel();

}

MainWindow::~MainWindow()
{
    m_singleChannel.stopChannel();
    delete ui;
}

