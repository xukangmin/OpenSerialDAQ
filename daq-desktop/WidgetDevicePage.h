#ifndef WIDGETDEVICEPAGE_H
#define WIDGETDEVICEPAGE_H

#include "Models.h"

#include <QTreeView>
#include <QWidget>

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
};

#endif // WIDGETDEVICEPAGE_H
