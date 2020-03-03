#ifndef WIDGETCHANNELLIST_H
#define WIDGETCHANNELLIST_H

#include <QScrollArea>
#include <QWidget>
#include "Channel/ChannelModel.h"
#include "WidgetChannel.h"
#include "Models.h"

namespace Ui {
class WidgetChannelList;
}

class WidgetChannelList : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetChannelList(QWidget *parent = nullptr);
    ~WidgetChannelList();

    void setModel(Models *models);
    void addNewChannel(QHash<QString, QVariant> properties);

private:
    Ui::WidgetChannelList *ui;
    ChannelModel* m_ch_model;
    DeviceModel * m_dev_model;
    Models *mModels;
    void addWidget(Models* models);

    void clearWidgets(QLayout * layout);

public slots:
    void updateWidgets();
};

#endif // WIDGETCHANNELLIST_H
