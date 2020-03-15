#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QListView>
#include <QMessageBox>
#include <QMetaType>
#include <QTableView>
#include "Channel/ChannelModel.h"
#include <memory>
#include <vector>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mStackedWidget(new QStackedWidget(this)),
      mWidgetChannelListPage(new WidgetChannelListPage(this)),
      mWidgetDevicePage(new WidgetDevicePage(this)),
      mWidgetStationPage(new WidgetStationPage(this)),
      mWidgetSettingPage(new WidgetSettingPage(this)),
      allModels(Models::instance())
{
    ui->setupUi(this);

 //   double test = UnitAndConversion::instance().voscocity(123,"Air");



    // Tool bar actions
    connect(ui->actionOverView,SIGNAL(triggered(bool)),this,SLOT(showOverViewPage()));
    connect(ui->actionDevices,SIGNAL(triggered(bool)),this,SLOT(showDevicePage()));
    connect(ui->actionStation,SIGNAL(triggered(bool)),this,SLOT(showStationPage()));
    connect(ui->actionSetting,SIGNAL(triggered(bool)),this,SLOT(showSettingPage()));

    connect(ui->actionAddNewDevice,SIGNAL(triggered(bool)),this,SLOT(showNewDeviceDialog()));
    connect(ui->actionTestButton2,SIGNAL(triggered(bool)),this,SLOT(triggerTestButton2()));
    connect(ui->actionTestButton3,SIGNAL(triggered(bool)),this,SLOT(triggerTestButton3()));
    connect(ui->actionTestButton4,SIGNAL(triggered(bool)),this,SLOT(triggerTestButton4()));
    // UI dialogs

    // Add new Channel UI
    connect(ui->actionAddNewChannel, SIGNAL(triggered()), this, SLOT(showNewChannelDialog()));



    if (allModels.mChannelModel->rowCount() == 0) {
        // populate test data

        QHash<QString, QVariant> properties;


        properties["id"] = 0;
        properties["ComPort"] = "COM3";
        properties["BaudRate"] = 9600;
        properties["DataBits"] = 8;
        properties["Parity"] = "None";
        properties["StopBits"] = 1;

        allModels.mChannelModel->addChannel(properties);
    }

    if (allModels.mDeviceModel->rowCount() == 0) {
        // populate test data

        QHash<QString, QVariant> properties;

        properties["Name"] = "ALICAT";
        properties["NodeID"] = 0;
        properties["Protocol"] = "LFE-ALICAT";
        properties["ChannelID"] = -1;

        allModels.mDeviceModel->addDevice(properties);


        properties["Name"] = "DP";
        properties["NodeID"] = 1;
        properties["Protocol"] = "LFE-DP";
        properties["ChannelID"] = -1;

        allModels.mDeviceModel->addDevice(properties);

    }

//    QTableView *tb = new QTableView(this);

//    tb->setModel(channelModel);

//    setCentralWidget(tb);


    mWidgetChannelListPage->setModel(&allModels);

    connect(allModels.mChannelModel, &ChannelModel::rowsInserted, mWidgetChannelListPage, &WidgetChannelListPage::updateWidgets);
    connect(allModels.mChannelModel, &ChannelModel::rowsRemoved, mWidgetChannelListPage, &WidgetChannelListPage::updateWidgets);

    mStackedWidget->addWidget(mWidgetChannelListPage);

    mStackedWidget->addWidget(mWidgetDevicePage);

    mStackedWidget->addWidget(mWidgetStationPage);

    mStackedWidget->addWidget(mWidgetSettingPage);

    setCentralWidget(mStackedWidget);

    showOverViewPage();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showDevicePage() {

    ui->toolBar->clear();

    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionStation);
    ui->toolBar->addAction(ui->actionSetting);

    ui->toolBar->addSeparator();

    ui->toolBar->addAction(ui->actionAddNewDevice);
    ui->toolBar->addAction(ui->actionTestButton2);
    ui->toolBar->addAction(ui->actionTestButton3);
    ui->toolBar->addAction(ui->actionTestButton4);

    mStackedWidget->setCurrentIndex(1);
}


void MainWindow::showOverViewPage() {

    ui->toolBar->clear();

    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionStation);
    ui->toolBar->addAction(ui->actionSetting);

    ui->toolBar->addSeparator();

    ui->toolBar->addAction(ui->actionAddNewChannel);

    mStackedWidget->setCurrentIndex(0);
}

void MainWindow::showStationPage() {

    ui->toolBar->clear();

    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionStation);
    ui->toolBar->addAction(ui->actionSetting);

    ui->toolBar->addSeparator();

    mStackedWidget->setCurrentWidget(mWidgetStationPage);
}

void MainWindow::showSettingPage() {

    ui->toolBar->clear();

    // setup toolbar
    ui->toolBar->addAction(ui->actionOverView);
    ui->toolBar->addAction(ui->actionDevices);
    ui->toolBar->addAction(ui->actionStation);
    ui->toolBar->addAction(ui->actionSetting);

    ui->toolBar->addSeparator();

    mStackedWidget->setCurrentWidget(mWidgetSettingPage);
}

void MainWindow::triggerTestButton1() {
    // add new Device
    Models::instance().mVariableGroupModel->loadGroupsFromConfigFile();
}


void MainWindow::triggerTestButton2() {
    shared_ptr<Variable> var;
    if (Models::instance().mVariableModel->findVariableByNameAndDeviceID("Pressure",1,var)) {
        QHash<QString,QVariant> data;

        data["TimeStamp"] = QDateTime::currentDateTime();
        data["Value"] = 2.3;
        var->addDataToVariable(data);
    }

}

void MainWindow::triggerTestButton3() {
    shared_ptr<Variable> var;
    if (Models::instance().mVariableModel->findVariableByNameAndDeviceID("Temperature",1,var)) {
        QHash<QString,QVariant> data;

        data["TimeStamp"] = QDateTime::currentDateTime();
        data["Value"] = 4.1;
        var->addDataToVariable(data);
    }
}

void MainWindow::triggerTestButton4() {
    shared_ptr<Variable> var;
    if (Models::instance().mVariableModel->findVariableByNameAndDeviceID("DP",2,var)) {
        QHash<QString,QVariant> data;

        data["TimeStamp"] = QDateTime::currentDateTime();
        data["Value"] = 6.1;
        var->addDataToVariable(data);
    }
}

void MainWindow::showNewDeviceDialog() {
    mDialogNewDevice = new DialogNewDevice(this);

    if (mDialogNewDevice->exec() == QDialog::Accepted) {
        QHash<QString, QVariant> properties = mDialogNewDevice->getDeviceInfo();

        if (!allModels.mDeviceModel->isDeviceExists(properties["Name"].toString(), properties["NodeID"].toInt()))
        {
            allModels.mDeviceModel->addDevice(properties);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Device Name And Node already exists");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }

    }

    mDialogNewDevice->deleteLater();
}

void MainWindow::showNewChannelDialog() {
    mDialogNewChannel = new DialogNewChannel(this);

    if (mDialogNewChannel->exec() == QDialog::Accepted) {
        QHash<QString, QVariant> properties = mDialogNewChannel->getChannelInfo();

        if (!allModels.mChannelModel->isPortExists(properties["ComPort"].toString())) {
            mWidgetChannelListPage->addNewChannel(properties);
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
