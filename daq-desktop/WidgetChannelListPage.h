#ifndef WIDGETCHANNELLISTPAGE_H
#define WIDGETCHANNELLISTPAGE_H

#include <QScrollArea>
#include <QWidget>
#include "Channel/ChannelModel.h"
#include "WidgetChannel.h"
#include "Models.h"

namespace Ui {
class WidgetChannelList;
}

class WidgetChannelListPage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetChannelListPage(QWidget *parent = nullptr);
    ~WidgetChannelListPage();

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

#endif // WIDGETCHANNELLISTPAGE_H
