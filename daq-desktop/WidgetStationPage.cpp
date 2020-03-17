#include "WidgetStationPage.h"
#include "ui_WidgetStationPage.h"
#include "Models.h"
#include "WidgetMiniVariable.h"

using namespace std;
WidgetStationPage::WidgetStationPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetStationPage)
{
    ui->setupUi(this);


    // display first Variable group
    int var_group_id = 5;

    mProxyModel = new VariableProxyModel(this);

    mProxyModel->setSourceModel(Models::instance().mVariableModel);

    mProxyModel->setGroupID(var_group_id);

    ui->tableView->setModel(mProxyModel);

}

WidgetStationPage::~WidgetStationPage()
{
    delete ui;
}
