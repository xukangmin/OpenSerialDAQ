#include "WidgetStationPage.h"
#include "ui_WidgetStationPage.h"
#include "Models.h"
#include "WidgetMiniVariable.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QQmlContext>


using namespace std;
WidgetStationPage::WidgetStationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStationPage),
    mSettings("Graftel", "OpenSerialDAQ")
{
    ui->setupUi(this);

    ui->lbProgress->setVisible(false);

    ui->progressBar->setVisible(false);

    connect(Models::instance().mVariableGroupModel, &VariableGroupModel::updateProgress, this, &WidgetStationPage::getProgress);
    connect(ui->btnSetupStation, &QPushButton::clicked, this, &WidgetStationPage::loadStation);

    int subStationSize = Models::instance().mVariableGroupModel->rowCount();

    if (subStationSize == 0) {
        ui->stackedWidget->setCurrentWidget(ui->emptyPage);    
    }
    else {
        QSettings settings("Graftel", "OpenSerialDAQ");

        ui->tabWidget->setTabText(0, settings.value("StationName").toString());

        setupStationQML(settings.value("StationType").toString());

        ui->stackedWidget->setCurrentWidget(ui->stationPage);
    }


}

WidgetStationPage::~WidgetStationPage()
{
    delete ui;
}

void WidgetStationPage::setVariableGroupIndex(int index)
{
    Models::instance().mVariableProxyModel->setGroupIndex(index);


    if (mSettings.value("DAQRunning").toInt() == 1) {
        Models::instance().mVariableGroupModel->endAllDAQ(1);

        QModelIndex ind = Models::instance().mVariableGroupModel->index(index,0);

        Models::instance().mVariableGroupModel->startDAQ(ind, 1);
    }

}

void WidgetStationPage::setupStationQML(QString stationType) {

    Models::instance().mVariableProxyModel->setSourceModel(Models::instance().mVariableModel);

    Models::instance().mVariableProxyModel->setGroupIndex(0);

    ui->tableView->setModel(Models::instance().mVariableProxyModel);

    ui->tableView->horizontalHeader()->show();

    ui->tableView->horizontalHeader()->setModel(Models::instance().mVariableProxyModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->verticalHeader()->hide();

    ui->tabWidget->setCurrentWidget(ui->display);

    QQmlContext *ctxt = ui->quickWidget->rootContext();

    ctxt->setContextProperty("vairableModel",Models::instance().mVariableModel);

    ctxt->setContextProperty("vairableModel",Models::instance().mVariableModel);

    ctxt->setContextProperty("variableGroupModel",Models::instance().mVariableGroupModel);

    ctxt->setContextProperty("variableProxyModel",Models::instance().mVariableProxyModel);

    ctxt->setContextProperty("unitAndConversion",&UnitAndConversion::instance());

    ctxt->setContextProperty("widgetStationPapge", this);

    ui->quickWidget->setSource(QUrl("qrc:/qml/" + stationType + ".qml"));
}

void WidgetStationPage::clearStation() {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Confirmation");
    msgBox.setInformativeText("Do you want to reset all configurations?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok:
          // Save was clicked
          Models::instance().mVariableGroupModel->removeAllRows();
          Models::instance().mVariableModel->removeAllRows();
          Models::instance().mDataModel->removeAllRows();
          Models::instance().mDeviceModel->removeAllRows();
          Models::instance().mChannelModel->removeAllRows();

          ui->stackedWidget->setCurrentWidget(ui->emptyPage);
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }

}

void WidgetStationPage::getProgress(QString progressName, int index, int total) {

    ui->lbProgress->setText(progressName);

    int prog = 100 * ((double)index + 1) / (double)total;

    ui->progressBar->setValue(prog);

}

void WidgetStationPage::loadStation() {
    // pop up file loading dialog
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Station Config File"), "",
        tr("Station Config File (*.json);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        ui->lbProgress->setVisible(true);
        ui->progressBar->setVisible(true);

        if (Models::instance().mVariableGroupModel->loadGroupsFromConfigFile(fileName)) {

            Models::instance().mVariableGroupModel->resolveDependency();

            emit sendMessage("Station Loaded.");
            ui->lbProgress->setVisible(false);
            ui->progressBar->setVisible(false);
            ui->tabWidget->setTabText(0, Models::instance().mVariableGroupModel->mStationName);


            mSettings.setValue("StationName",Models::instance().mVariableGroupModel->mStationName);
            mSettings.setValue("StationVersion",Models::instance().mVariableGroupModel->mStationVersion);
            mSettings.setValue("StationType",Models::instance().mVariableGroupModel->mStationType);

            setupStationQML(Models::instance().mVariableGroupModel->mStationType);

            ui->stackedWidget->setCurrentWidget(ui->stationPage);
        }
        else {
            emit sendMessage("Load Station Config Failed!!");
        }

        ui->lbProgress->setVisible(false);
        ui->progressBar->setVisible(false);
    }
}
