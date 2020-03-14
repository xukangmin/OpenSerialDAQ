#ifndef WIDGETDEVICEPAGE_H
#define WIDGETDEVICEPAGE_H

#include "Models.h"
#include "WidgetDevice.h"
#include "WidgetVariable.h"

#include <QTreeView>
#include <QWidget>
#include <ui_WidgetDevice.h>

#include <Channel/ChannelModel.h>

#include <Device/DeviceModel.h>

namespace Ui {
class WidgetDevicePage;
}

class WidgetDevicePage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDevicePage(QWidget *parent = nullptr);
    ~WidgetDevicePage();

private:
    Ui::WidgetDevicePage *ui;
    QItemSelectionModel* mSelectionModel;
    QVector<WidgetDevice*> mWidgetDeviceList;
    QVector<WidgetVariable*> mWidgetVariableList;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // WIDGETDEVICEPAGE_H
