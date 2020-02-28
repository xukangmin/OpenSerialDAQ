#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "Channel/Channel.h"

namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    ChannelWidget(Channel ch, QWidget *parent = nullptr);
    Channel m_ch;
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

signals:
    void deleteChannel(int id);
    void startChannel(int id);
    void stopChannel(int id);
    void addDeviceToChannel(int id);
};

#endif // CHANNELWIDGET_H
