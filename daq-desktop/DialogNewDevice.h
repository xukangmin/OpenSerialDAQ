#ifndef DIALOGADDNEWDEVICE_H
#define DIALOGADDNEWDEVICE_H

#include <QDialog>

namespace Ui {
class DialogNewDevice;
}

class DialogNewDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewDevice(QWidget *parent = nullptr);
    ~DialogNewDevice();

    QHash<QString, QVariant> getDeviceInfo();

private:
    Ui::DialogNewDevice *ui;
};
#endif // DIALOGADDNEWDEVICE_H
