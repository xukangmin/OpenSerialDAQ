#ifndef MINIDEVICEWIDGET_H
#define MINIDEVICEWIDGET_H

#include "Models.h"

#include <QWidget>

#include <Device/DeviceModel.h>

namespace Ui {
class MiniDeviceWidget;
}

class MiniDeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MiniDeviceWidget(Models *models, QWidget *parent = nullptr);
    ~MiniDeviceWidget();

    DeviceModel* m_dev_model;
    QModelIndex m_dev_index;
    Models *mModels;

private slots:
    void on_btnDelete_clicked();

private:
    Ui::MiniDeviceWidget *ui;

signals:
    void deleteDeviceFromChannel(int dev_id);
};

#endif // MINIDEVICEWIDGET_H
