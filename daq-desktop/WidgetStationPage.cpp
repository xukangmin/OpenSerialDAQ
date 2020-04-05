#include "WidgetStationPage.h"
#include "ui_WidgetStationPage.h"
#include "Models.h"
#include "WidgetMiniVariable.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QQmlContext>
#include <QQuickStyle>

using namespace std;
WidgetStationPage::WidgetStationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStationPage),
    mSettings("Graftel", "OpenSerialDAQ"),
    mWidgetVariableChart(nullptr)
{
    ui->setupUi(this);

    QQuickStyle::setStyle("Fusion");

    ui->lbProgress->setVisible(false);

    ui->progressBar->setVisible(false);

    connect(Models::instance().mVariableGroupModel, &VariableGroupModel::updateProgress, this, &WidgetStationPage::getProgress);

    connect(ui->btnSetupStation, &QPushButton::clicked, this, &WidgetStationPage::loadStation);

    connect(ui->tableView, &QTableView::doubleClicked, this, &WidgetStationPage::enableDetailView);

    connect(ui->btnValidate, &QPushButton::clicked, this, &WidgetStationPage::validateEquations);

    connect(ui->btnValidateClear, &QPushButton::clicked, this, &WidgetStationPage::clearValidation);

    int subStationSize = Models::instance().mVariableGroupModel->rowCount();

    if (subStationSize == 0) {
        ui->stackedWidget->setCurrentWidget(ui->emptyPage);    
    }
    else {

        ui->tabWidget->setTabText(0, mSettings.value("StationName").toString());

        setupStationQML(mSettings.value("StationType").toString());

        ui->stackedWidget->setCurrentWidget(ui->stationPage);
    }


}

WidgetStationPage::~WidgetStationPage()
{
    delete ui;
}
void WidgetStationPage::clearValidation() {

    shared_ptr<VariableGroup> varG;

    if (Models::instance().mVariableGroupModel->findVariableGroupByName("Validation",varG)) {
        Models::instance().mVariableGroupModel->removeByID(varG->m_id);
    }
}

void WidgetStationPage::validateEquations() {
    // first creat variable group using validation values
    QString stationType = mSettings.value("StationType").toString();

    Models::instance().mVariableGroupModel->loadValidationGroups(stationType, 0);

    // add data to Variable Model
    Models::instance().mVariableModel->addValidationData();

    shared_ptr<VariableGroup> varG;

    Models::instance().mVariableGroupModel->findVariableGroupByName("Validation",varG);

    Models::instance().mVariableValidationModel->setGroupID(varG->m_id);

    if (Models::instance().mVariableGroupModel->findVariableGroupByName("Validation",varG))
    {
        Models::instance().mVariableValidationModel->setSourceModel(Models::instance().mVariableModel);

        Models::instance().mVariableValidationModel->setGroupID(varG->m_id);

        ui->tableViewValidate->setModel(Models::instance().mVariableValidationModel);

        ui->tableViewValidate->horizontalHeader()->show();

        ui->tableViewValidate->horizontalHeader()->setModel(Models::instance().mVariableValidationModel);
    }

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

void WidgetStationPage::enableDetailView(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    // find related variable, if variable is not user input or constant, open a detailed chart

    prev_index = curr_index;
    curr_index = Models::instance().mVariableProxyModel->mapToSource(index);


    shared_ptr<Variable> var;

    if (Models::instance().mVariableModel->getVariableByIndex(curr_index, var)) {

        if (var->getSingleProperty("Type").toString() != "UserInput" &&
            var->getSingleProperty("Type").toString() != "Constant" &&
            var->getSingleProperty("DataType").toString() == "double" &&
            var->historyData.size() > 0)
        {
            if (prev_index != curr_index)
            {
                if (mWidgetVariableChart == nullptr) {
                    mWidgetVariableChart = new WidgetVariableChart(var, this);
                    ui->verticalLayout_4->addWidget(mWidgetVariableChart);
                }
                else {
                    mWidgetVariableChart->setVariable(var);
                }
            }
        }


    }


    qDebug() << index.row();
    qDebug() << index.column();
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

    shared_ptr<VariableGroup> varG;

    if (Models::instance().mVariableGroupModel->findVariableGroupByName("Validation",varG))
    {
        Models::instance().mVariableValidationModel->setSourceModel(Models::instance().mVariableModel);

        Models::instance().mVariableValidationModel->setGroupID(varG->m_id);

        ui->tableViewValidate->setModel(Models::instance().mVariableValidationModel);

        ui->tableViewValidate->horizontalHeader()->show();

        ui->tableView->horizontalHeader()->setModel(Models::instance().mVariableValidationModel);
    }

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

            //Models::instance().mVariableGroupModel->resolveAllDependency();

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
