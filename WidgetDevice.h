#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include "Device.h"
#include "Channel.h"

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(Device *dev, QVector<Channel> ch_list, QWidget *parent = nullptr);
    ~DeviceWidget();

    Device *m_dev;

    QVector<Channel> m_ch_list;

private:
    Ui::DeviceWidget *ui;
};

#endif // DEVICEWIDGET_H
