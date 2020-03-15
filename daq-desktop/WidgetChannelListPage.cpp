#include "WidgetChannelListPage.h"
#include "ui_WidgetChannelList.h"
#include "WidgetChannel.h"

WidgetChannelListPage::WidgetChannelListPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChannelList)
{
    ui->setupUi(this);

    ui->verticalLayout->setAlignment(Qt::AlignTop);
}

void WidgetChannelListPage::clearWidgets(QLayout * layout) {
   if (! layout)
      return;
   while (auto item = layout->takeAt(0)) {
      delete item->widget();
      clearWidgets(item->layout());
   }
}

void WidgetChannelListPage::updateWidgets() {
    clearWidgets(ui->verticalLayout);

    for(int i = 0; i < m_ch_model->rowCount(); i++) {
        mModels->mChannelIndex = mModels->mChannelModel->index(i,0);
        addWidget(mModels);
    }
}

void WidgetChannelListPage::addWidget(Models* models) {
    auto ch_widget = new ChannelWidget(models, this);
    ui->verticalLayout->addWidget(ch_widget);
}

void WidgetChannelListPage::setModel(Models *models)
{
    m_ch_model = models->mChannelModel;
    m_dev_model = models->mDeviceModel;
    mModels = models;
    updateWidgets();
}

void WidgetChannelListPage::addNewChannel(QHash<QString, QVariant> properties) {
    m_ch_model->addChannel(properties);
    updateWidgets();
}

WidgetChannelListPage::~WidgetChannelListPage()
{
    delete ui;
}
