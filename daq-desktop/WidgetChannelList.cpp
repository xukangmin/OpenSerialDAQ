#include "WidgetChannelList.h"
#include "ui_WidgetChannelList.h"
#include "WidgetChannel.h"

WidgetChannelList::WidgetChannelList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChannelList)
{
    ui->setupUi(this);
}

void WidgetChannelList::clearWidgets(QLayout * layout) {
   if (! layout)
      return;
   while (auto item = layout->takeAt(0)) {
      delete item->widget();
      clearWidgets(item->layout());
   }
}

void WidgetChannelList::updateWidgets() {
    clearWidgets(ui->verticalLayout);

    for(int i = 0; i < m_model->rowCount(); i++) {
        addWidget(m_model,i);
    }
}

void WidgetChannelList::addWidget(ChannelModel *model, int row_index) {
    auto ch_widget = new ChannelWidget(model, row_index, this);
    ui->verticalLayout->addWidget(ch_widget);
}

void WidgetChannelList::setModel(ChannelModel *model)
{
    m_model = model;

    updateWidgets();
}

void WidgetChannelList::addNewChannel(QHash<QString, QVariant> properties) {
    m_model->addChannel(properties);
    updateWidgets();
}

WidgetChannelList::~WidgetChannelList()
{
    delete ui;
}
