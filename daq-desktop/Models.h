#ifndef MODELS_H
#define MODELS_H

#include <Channel/ChannelModel.h>

#include <Device/DeviceModel.h>



class Models
{
public:
    Models(ChannelModel *channelModel, DeviceModel *deviceModel);

    ChannelModel *mChannelModel;
    DeviceModel *mDeviceModel;

    QModelIndex mChannelIndex;
    QModelIndex mDeviceIndex;
};

#endif // MODELS_H
