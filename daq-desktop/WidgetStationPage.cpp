#include "WidgetStationPage.h"
#include "ui_WidgetStationPage.h"
#include "Models.h"
#include "WidgetMiniVariable.h"

#include <QQmlContext>

using namespace std;
WidgetStationPage::WidgetStationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStationPage)
{
    ui->setupUi(this);


    // display first Variable group
//    int var_group_id = 1;

    mProxyModel = new VariableProxyModel(this);

    mProxyModel->setSourceModel(Models::instance().mVariableModel);

    mProxyModel->setGroupIndex(0);

    ui->tableView->setModel(mProxyModel);

    ui->tableView->horizontalHeader()->show();
    ui->tableView->verticalHeader()->hide();

    ui->tabWidget->setCurrentWidget(ui->display);

    QQmlContext *ctxt = ui->quickWidget->rootContext();

    ctxt->setContextProperty("vairableModel",Models::instance().mVariableModel);

    ctxt->setContextProperty("vairableModel",Models::instance().mVariableModel);

    ctxt->setContextProperty("variableGroupModel",Models::instance().mVariableGroupModel);

    ctxt->setContextProperty("variableProxyModel",mProxyModel);

    ctxt->setContextProperty("unitAndConversion",&UnitAndConversion::instance());


//    qmlRegisterSingletonType<UnitAndConversion>("UnitAndConversion", 1, 0, "UnitApi", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
//        Q_UNUSED(engine)
//        Q_UNUSED(scriptEngine)

//        return &UnitAndConversion::instance();
//    });

     ui->quickWidget->setSource(QUrl("qrc:/qml/station.qml"));




}

WidgetStationPage::~WidgetStationPage()
{
    delete ui;
}
