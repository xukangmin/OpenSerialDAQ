#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_singleChannel.startChannel();

}

MainWindow::~MainWindow()
{
    m_singleChannel.stopChannel();
    delete ui;
}

