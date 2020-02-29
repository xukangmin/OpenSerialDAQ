#include "WidgetChannelList.h"
#include "ui_WidgetChannelList.h"
#include "WidgetChannel.h"

WidgetChannelList::WidgetChannelList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChannelList)
{
    ui->setupUi(this);
}

void WidgetChannelList::setModel(ChannelModel *model)
{
    m_model = model;
    // populate small widgets

    auto chs = m_model->getAllChannels();

    foreach(auto ch, chs) {
        auto ch_widget = new ChannelWidget(*ch,this);
        connect(ch_widget, &ChannelWidget::deleteChannel, this, &WidgetChannelList::deleteChannel);
//        connect(ch_widget, &ChannelWidget::startChannel, this, &WidgetChannelList::startChannel);
//        connect(ch_widget, &ChannelWidget::stopChannel, this, &WidgetChannelList::stopChannel);
        ui->verticalLayout->addWidget(ch_widget);
    }
//    auto list = model->getAllChannels();

//    foreach(auto c, list) {
//        auto ch_widget = new ChannelWidget(c,this);
//        connect(ch_widget, &ChannelWidget::deleteChannel, this, &WidgetChannelList::deleteChannel);
//        ui->verticalLayout->addWidget(ch_widget);
//    }

}

void WidgetChannelList::addNewChannel(Channel channel) {
    m_model->addChannel(channel);

    auto ch_widget = new ChannelWidget(channel,this);
    connect(ch_widget, &ChannelWidget::deleteChannel, this, &WidgetChannelList::deleteChannel);
//    connect(ch_widget, &ChannelWidget::startChannel, this, &WidgetChannelList::startChannel);
//    connect(ch_widget, &ChannelWidget::stopChannel, this, &WidgetChannelList::stopChannel);
    ui->verticalLayout->addWidget(ch_widget);
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
