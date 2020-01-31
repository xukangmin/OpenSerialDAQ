#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "channel.h"

namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    ChannelWidget(Channel ch, QWidget *parent = nullptr);
    Channel m_ch;
    ~ChannelWidget();

private slots:
    void on_btnDelete_clicked();

    void on_btnStart_clicked();

private:

    Ui::ChannelWidget *ui;

signals:
    void deleteChannel(int id);
    void startChannel(int id);
};

#endif // CHANNELWIDGET_H
