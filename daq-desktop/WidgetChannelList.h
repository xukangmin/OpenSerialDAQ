#ifndef WIDGETCHANNELLIST_H
#define WIDGETCHANNELLIST_H

#include <QScrollArea>
#include <QWidget>
#include "Channel/ChannelModel.h"
#include "WidgetChannel.h"

namespace Ui {
class WidgetChannelList;
}

class WidgetChannelList : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetChannelList(QWidget *parent = nullptr);
    ~WidgetChannelList();

    void setModel(ChannelModel* model);
    void addNewChannel(Channel channel);

private:
    Ui::WidgetChannelList *ui;
    ChannelModel* m_model;
    void updateWidgets();
    void addWidget(Channel ch);
    void addNewChannelWidget();
private slots:
    void deleteChannel(int id);
    void startChannel(int id);
    void stopChannel(int id);
};

#endif // WIDGETCHANNELLIST_H
