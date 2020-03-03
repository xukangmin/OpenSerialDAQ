#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "Channel/ChannelModel.h"
#include "DialogAddDeviceToChannel.h"
#include "Models.h"

namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    ChannelWidget(Models* models, QWidget *parent = nullptr);
    ChannelModel* m_ch_model;
    int m_row_index;
    DeviceModel* m_dev_model;
    Models* mModels;
    void addDeviceToChannel(const QModelIndex& dev_index);
    int getChannelID();
    //void addMiniDeviceWidget(MiniDeviceWidget* dev);
    ~ChannelWidget();

public slots:
//    void removeMiniDeviceWidget(int dev_id);

private slots:
    void on_btnDelete_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnAddDevice_clicked();


private:
    Ui::ChannelWidget *ui;
    DialogAddDeviceToChannel* mDialogAddDevice;
};

#endif // CHANNELWIDGET_H
