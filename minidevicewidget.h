#ifndef MINIDEVICEWIDGET_H
#define MINIDEVICEWIDGET_H

#include <QWidget>

namespace Ui {
class MiniDeviceWidget;
}

class MiniDeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MiniDeviceWidget(QString devName, int devID, QWidget *parent = nullptr);
    ~MiniDeviceWidget();

    QString m_devName;
    int m_devID;

private slots:
    void on_btnDelete_clicked();

private:
    Ui::MiniDeviceWidget *ui;

signals:
    void deleteDeviceFromChannel(int dev_id);
};

#endif // MINIDEVICEWIDGET_H
