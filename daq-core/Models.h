#ifndef MODELS_H
#define MODELS_H

#include <Channel/ChannelModel.h>

#include <Device/DeviceModel.h>

#include <Variable/VariableModel.h>

#include <Data/DataModel.h>

#include <Unit/UnitModel.h>

#include <UnitType/UnitTypeModel.h>

#include <UnitConversion/UnitConversionModel.h>


class DAQCORESHARED_EXPORT Models : public QObject
{

Q_OBJECT

public:
    static Models& instance();
    ~Models();

    ChannelModel *mChannelModel;
    DeviceModel *mDeviceModel;
    VariableModel *mVariableModel;
    DataModel *mDataModel;
    UnitModel *mUnitModel;
    UnitTypeModel *mUnitTypeModel;
    UnitConversionModel *mUnitConversionModel;


    QModelIndex mChannelIndex;
    QModelIndex mDeviceIndex;
protected:
    Models(QObject* parent = nullptr);


};

#endif // MODELS_H
