#ifndef WIDGETDEVICE_H
#define WIDGETDEVICE_H

#include <QWidget>

namespace Ui {
class WidgetDevice;
}

class WidgetDevice : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDevice(QWidget *parent = nullptr);
    ~WidgetDevice();

private:
    Ui::WidgetDevice *ui;
};

#endif // WIDGETDEVICE_H
