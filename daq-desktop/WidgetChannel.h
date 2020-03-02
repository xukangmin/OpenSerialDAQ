#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "Channel/ChannelModel.h"

namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    ChannelWidget(ChannelModel* model, int row_index , QWidget *parent = nullptr);
    ChannelModel* m_model;
    int m_row_index;
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

};

#endif // CHANNELWIDGET_H
