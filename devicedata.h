#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QVector>

class DeviceData
{
public:
    DeviceData();

    QVector<QVector<double>> historyData;

    QVector<QVector<unsigned long>> timeStamp;

    QVector<double> currentData;

    int dataTypeCount;

    int totalDataCount;
};

#endif // DEVICEDATA_H
