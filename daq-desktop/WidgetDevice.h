#ifndef WIDGETDEVICE_H
#define WIDGETDEVICE_H

#include <QWidget>

#include <Device/Device.h>

namespace Ui {
class WidgetDevice;
}

class WidgetDevice : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDevice(Device* dev, QWidget *parent = nullptr);
    ~WidgetDevice();

    int mDevID;

private:
    Ui::WidgetDevice *ui;
    Device* mDevice;

};

#endif // WIDGETDEVICE_H
