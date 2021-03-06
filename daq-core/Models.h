#ifndef MODELS_H
#define MODELS_H

#include <Channel/ChannelModel.h>

#include <Device/DeviceModel.h>

#include <Variable/VariableModel.h>

#include <VariableGroup/VariableGroupModel.h>

#include <Data/DataModel.h>

#include <QMutex>



class DAQCORESHARED_EXPORT Models : public QObject
{

Q_OBJECT

public:
    static Models& instance();
    ~Models();

    QMutex mutex_global;
    ChannelModel *mChannelModel;
    DeviceModel *mDeviceModel;
    VariableModel *mVariableModel;
    DataModel *mDataModel;
    VariableGroupModel *mVariableGroupModel;
    VariableProxyModel *mVariableProxyModel;
    VariableValidationModel *mVariableValidationModel;
    QModelIndex mChannelIndex;
    QModelIndex mDeviceIndex;
protected:
    Models(QObject* parent = nullptr);


};

#endif // MODELS_H
