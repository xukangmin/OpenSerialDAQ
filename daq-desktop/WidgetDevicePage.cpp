#include "WidgetDevicePage.h"
#include "ui_WidgetDevicePage.h"

#include "Models.h"
#include <QDebug>
#include "Device/Device.h"
#include "Variable/Variable.h"

using namespace std;

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

        auto it = find_if(mWidgetDeviceList.begin(), mWidgetDeviceList.end(), [&dev] (WidgetDevice* wd) {
            return dev->m_id == wd->mDevID;
        });

        if (it == mWidgetDeviceList.end()) {
            WidgetDevice* wd = new WidgetDevice(dev, this);
            mWidgetDeviceList.append(wd);

            this->ui->stackedWidget->addWidget(wd);
            this->ui->stackedWidget->setCurrentWidget(wd);
        }
        else {
            this->ui->stackedWidget->setCurrentWidget(*it);
        }


    }
    else if (obj->property("ObjType").toString() == "variable")
    {
        Variable* var = static_cast<Variable*>(obj);


        auto it = find_if(mWidgetVariableList.begin(), mWidgetVariableList.end(), [&var] (WidgetVariable* wv) {
            return var->m_id == wv->m_var_id;
        });

        if (it == mWidgetVariableList.end()) {
            WidgetVariable* wv = new WidgetVariable(var, this);
            mWidgetVariableList.append(wv);

            this->ui->stackedWidget->addWidget(wv);
            this->ui->stackedWidget->setCurrentWidget(wv);
        }
        else {
            this->ui->stackedWidget->setCurrentWidget(*it);
        }
    }
}
