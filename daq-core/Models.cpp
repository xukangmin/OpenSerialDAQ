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
    mUnitModel->deleteLater();
}

Models::Models(QObject *parent) :
    mChannelModel(new ChannelModel(this)),
    mDeviceModel(new DeviceModel(this)),
    mVariableModel(new VariableModel(mDeviceModel,this)),
    mDataModel(new DataModel(this)),
    mUnitModel(new UnitModel(this))
{
}
