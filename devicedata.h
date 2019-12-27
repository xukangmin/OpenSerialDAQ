#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QVector>

class DeviceData
{
public:
    DeviceData();

    QVector<double> historyData;
    double currentData;

};

#endif // DEVICEDATA_H
