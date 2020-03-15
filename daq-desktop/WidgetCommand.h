#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H

#include <QGroupBox>

#include <Device/Device.h>

namespace Ui {
class WidgetCommand;
}

class WidgetCommand : public QGroupBox
{
    Q_OBJECT

public:
    explicit WidgetCommand(Device* dev, int cmdIndex, QWidget *parent = nullptr);
    ~WidgetCommand();

private:
    Ui::WidgetCommand *ui;
    Device* mDevice;
    Command mCmd;
};

#endif // COMMANDWIDGET_H
