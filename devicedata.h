#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QDateTime>
#include <QVector>
#include <QMetaType>

struct currentData{
    QVector<double> values;
    QDateTime timestamp;
    QVector<QString> name;
    QVector<QString> unit;
};

Q_DECLARE_METATYPE(currentData)

class DeviceData
{
public:
    DeviceData();

    QVector<QVector<double>> historyData;

    QVector<QDateTime> timeStamp;

    QVector<double> currentData;

    QDateTime currentTimeStamp;

    QVector<QString> dataName;

    QVector<QString> dataUnit;

    int totalDataCount;
};

#endif // DEVICEDATA_H
