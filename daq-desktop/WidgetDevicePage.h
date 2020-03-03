#ifndef WIDGETDEVICEPAGE_H
#define WIDGETDEVICEPAGE_H

#include "Models.h"

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

    void setModel(Models* models);
private:
    ChannelModel *m_ch_model;
    DeviceModel *m_dev_model;
    Ui::WidgetDevicePage *ui;
};

#endif // WIDGETDEVICEPAGE_H
