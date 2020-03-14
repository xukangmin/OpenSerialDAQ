#include "WidgetDevicePage.h"
#include "ui_WidgetDevicePage.h"

#include "Models.h"
#include <QDebug>
#include "Device/Device.h"
#include "Variable/Variable.h"

WidgetDevicePage::WidgetDevicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDevicePage)
{
    ui->setupUi(this);

    ui->treeView->setModel(Models::instance().mDeviceModel);

    mSelectionModel =  ui->treeView->selectionModel();

    connect(mSelectionModel,&QItemSelectionModel::selectionChanged,this,&WidgetDevicePage::selectionChanged);
}


WidgetDevicePage::~WidgetDevicePage()
{
    delete ui;
}

void WidgetDevicePage::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected)
    QModelIndex index = selected.indexes().at(0);
    QObject* obj = static_cast<QObject*>(index.internalPointer());

    if (obj->property("ObjType").toString() == "device")
    {
        Device* dev = static_cast<Device*>(obj);
        qDebug() << dev->getSingleProperty("Name");
        qDebug() << dev->getSingleProperty("NodeID");
        qDebug() << dev->getSingleProperty("Protocol");
    }
    else if (obj->property("ObjType").toString() == "variable")
    {
        Variable* var = static_cast<Variable*>(obj);
        qDebug() << var->getSingleProperty("Name");
        qDebug() << var->getSingleProperty("DeviceID");
        qDebug() << var->getSingleProperty("Unit");
    }
}
