#include "Models.h"

Models& Models::instance()
{
    static Models singleton;
    return singleton;
}

Models::~Models() {
    mChannelModel->deleteLater();
    mDeviceModel->deleteLater();
    mVariableModel->deleteLater();
    mDataModel->deleteLater();
    mVariableGroupModel->deleteLater();
}

Models::Models(QObject *parent) :
    mChannelModel(new ChannelModel(parent)),
    mDeviceModel(new DeviceModel(parent)),
    mVariableModel(new VariableModel(mDeviceModel,parent)),
    mDataModel(new DataModel(parent)),
    mVariableGroupModel(new VariableGroupModel(mVariableModel,parent)),
    mVariableProxyModel(new VariableProxyModel(parent)),
    mVariableValidationModel(new VariableProxyModel(parent))
{
}
