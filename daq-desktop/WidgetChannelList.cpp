#include "WidgetChannelList.h"
#include "ui_WidgetChannelList.h"
#include "WidgetChannel.h"

WidgetChannelList::WidgetChannelList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChannelList)
{
    ui->setupUi(this);
}

void clearWidgets(QLayout * layout) {
   if (! layout)
      return;
   while (auto item = layout->takeAt(0)) {
      delete item->widget();
      clearWidgets(item->layout());
   }
}

void WidgetChannelList::addWidget(Channel ch) {
    auto ch_widget = new ChannelWidget(ch,this);
    connect(ch_widget, &ChannelWidget::deleteChannel, this, &WidgetChannelList::deleteChannel);
    ui->verticalLayout->addWidget(ch_widget);
}

void WidgetChannelList::updateWidgets() {


}

void WidgetChannelList::setModel(ChannelModel *model)
{
    m_model = model;
    auto chs = m_model->getAllChannels();

    foreach(auto ch, chs) {
        addWidget(*ch);
    }

}

void WidgetChannelList::addNewChannel(Channel channel) {
    m_model->addChannel(channel);
    addWidget(channel);
}

void WidgetChannelList::deleteChannel(int id) {
    m_model->removeByID(id);
}

void WidgetChannelList::startChannel(int id) {
//    m_model->startChannel(id);
}

void WidgetChannelList::stopChannel(int id) {
//    m_model->stopChannel(id);
}


WidgetChannelList::~WidgetChannelList()
{
    delete ui;
}
