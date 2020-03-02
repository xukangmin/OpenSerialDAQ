#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QListView>
#include <QMessageBox>
#include <QMetaType>
#include <QTableView>
#include "Channel/ChannelModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mStackedWidget(new QStackedWidget(this)),
      m_widgetChannelList(new WidgetChannelList(this))
{
    ui->setupUi(this);

    // Tool bar actions
    connect(ui->actionOverView,SIGNAL(triggered(bool)),this,SLOT(showOverViewPage()));
    connect(ui->actionDevices,SIGNAL(triggered(bool)),this,SLOT(showDevicePage()));


    // UI dialogs

    // Add new Channel UI
    connect(ui->actionAddNewChannel, SIGNAL(triggered()), this, SLOT(showNewChannelDialog()));

    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionAddNewChannel);

    // Generate Channel Widges
    channelModel = new ChannelModel(this);

    if (channelModel->rowCount() == 0) {
        // populate test data

        QHash<QString, QVariant> properties;


        properties["id"] = 0;
        properties["ComPort"] = "COM3";
        properties["BaudRate"] = 9600;
        properties["DataBits"] = 8;
        properties["Parity"] = "None";
        properties["StopBits"] = 1;

        channelModel->addChannel(properties);
    }

    deviceModel = new DeviceModel(this);

    if (deviceModel->rowCount() == 0) {
        // populate test data

        QHash<QString, QVariant> properties;

        properties["Name"] = "ALICAT";
        properties["NodeID"] = 1;
        properties["Protocol"] = "LFE-ALICAT";
        properties["ChannelID"] = 0;

        deviceModel->addDevice(properties);

    }

//    QTableView *tb = new QTableView(this);

//    tb->setModel(channelModel);

//    setCentralWidget(tb);


    m_widgetChannelList->setModel(channelModel);

    connect(channelModel, &ChannelModel::rowsInserted, m_widgetChannelList, &WidgetChannelList::updateWidgets);
    connect(channelModel, &ChannelModel::rowsRemoved, m_widgetChannelList, &WidgetChannelList::updateWidgets);


    mStackedWidget->addWidget(m_widgetChannelList);

    setCentralWidget(mStackedWidget);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showDevicePage() {
    mStackedWidget->setCurrentIndex(1);
}


void MainWindow::showOverViewPage() {
    mStackedWidget->setCurrentIndex(0);
}

void MainWindow::showNewChannelDialog() {
    mDialogNewChannel = new DialogNewChannel(this);

    if (mDialogNewChannel->exec() == QDialog::Accepted) {
        QHash<QString, QVariant> properties = mDialogNewChannel->getChannelInfo();

        if (!channelModel->isPortExists(properties["ComPort"].toString())) {
            m_widgetChannelList->addNewChannel(properties);
        } else {
            QMessageBox msgBox;
            msgBox.setText("Same Com port already exists!");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }

    mDialogNewChannel->deleteLater();

//    if (mDialogNewChannel->exec() == QDialog::Accepted) {

//        Channel chinfo(mDialogNewChannel->getChannelInfo());

//        bool exists = false;
//        foreach(ThreadChannel* ch, m_Channels) {
//            if (ch->m_ch.m_portName == chinfo.m_portName) {
//                exists = true;
//            }
//        }

//        if (!exists) {
//            createNewChannel(chinfo);
//        } else {
//            QMessageBox msgBox;
//            msgBox.setText("Same Com port already exists!");
//            msgBox.setIcon(QMessageBox::Warning);
//            msgBox.exec();
//        }



//    }

//    m_dlgNewChennel->deleteLater();
}
