#include "WidgetStationPage.h"
#include "ui_WidgetStationPage.h"
#include "Models.h"
#include "WidgetMiniVariable.h"

#include <QFileDialog>
#include <QQmlContext>

using namespace std;
WidgetStationPage::WidgetStationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStationPage)
{
    ui->setupUi(this);


    int subStationSize = Models::instance().mVariableGroupModel->rowCount();

    if (subStationSize == 0) {
        ui->stackedWidget->setCurrentWidget(ui->emptyPage);
        connect(ui->btnSetupStation, &QPushButton::clicked, this, &WidgetStationPage::loadStation);
    }
    else {
        ui->stackedWidget->setCurrentWidget(ui->stationPage);

        mProxyModel = new VariableProxyModel(this);

        mProxyModel->setSourceModel(Models::instance().mVariableModel);

        mProxyModel->setGroupIndex(0);

        ui->tableView->setModel(mProxyModel);

        ui->tableView->horizontalHeader()->show();

        ui->tableView->horizontalHeader()->setModel(mProxyModel);

        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        ui->tableView->verticalHeader()->hide();

        ui->tabWidget->setCurrentWidget(ui->display);

        QQmlContext *ctxt = ui->quickWidget->rootContext();

        ctxt->setContextProperty("vairableModel",Models::instance().mVariableModel);

        ctxt->setContextProperty("vairableModel",Models::instance().mVariableModel);

        ctxt->setContextProperty("variableGroupModel",Models::instance().mVariableGroupModel);

        ctxt->setContextProperty("variableProxyModel",mProxyModel);

        ctxt->setContextProperty("unitAndConversion",&UnitAndConversion::instance());

        ui->quickWidget->setSource(QUrl("qrc:/qml/LaminarStation.qml"));
    }


}

WidgetStationPage::~WidgetStationPage()
{
    delete ui;
}

void WidgetStationPage::clearStation() {
    //Models::instance().mVariableGroupModel->removeRows(0,Models::instance().mVariableGroupModel->rowCount());
    ui->stackedWidget->setCurrentWidget(ui->emptyPage);
}

void WidgetStationPage::loadStation() {
    // pop up file loading dialog
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Station Config File"), "",
        tr("Station Config File (*.json);;All Files (*)"));

    qDebug() << fileName;
}
