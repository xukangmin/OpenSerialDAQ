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
    void addNewChannel(QHash<QString, QVariant> properties);

private:
    Ui::WidgetChannelList *ui;
    ChannelModel* m_model;
    void addWidget(ChannelModel *model, int row_index);

    void clearWidgets(QLayout * layout);

public slots:
    void updateWidgets();
};

#endif // WIDGETCHANNELLIST_H
