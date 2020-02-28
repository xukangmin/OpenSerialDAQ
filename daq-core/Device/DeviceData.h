#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>

struct currentData{
    QVector<double> values;
    QDateTime timestamp;
    QVector<QString> name;
    QVector<QString> unit;
};

Q_DECLARE_METATYPE(currentData)

enum DataStatus
{
    NotAvailable,
    Available
};

enum DataPhysics
{
    Flow1,
    Temperature1,
    Humidity1,
    Pressure1,
    Voltage1,
    None
};

enum HumidityUnits{
    percent
};

enum FlowUnits
{
    sccm,
    slpm,
    scfm,
    scfh,
    slph
};

enum PressureUnits
{
    psig,
    ksi,
    bar,
    psia
};

enum TemperatureUnits
{
    C,
    F,
    K,
    R
};

static const QString strFlowRateUnits[] = {"sccm","slpm","scfm","scfh","slph"};

static const QString strVolFlowRateUnits[] = {"ccm","lpm","cfm","cfh","lph"};

static const QString strPressureUnits[] = {"psig","ksi","bar","psia"};

static const QString strTemperatureUnits[] = {"C","F","K","R"};

static const int MAX_HISTORY_DATA_SIZE = 10;

class DeviceData
{
public:
    DeviceData();

    DeviceData(int dataIndex,
               QString name,
               int cmd_id = 0,
               DataPhysics ph = None,
               FlowUnits flowUnit = sccm,
               TemperatureUnits tempUnit = C,
               PressureUnits pressUnit = psia,
               int paraIndex = 0,
               double dataMax = std::numeric_limits<double>::max(),
               double dataMin = std::numeric_limits<double>::min());

    void addData(double data, QDateTime curTime = QDateTime::currentDateTime());

    int m_dataIndex;

    QString m_dataName;

    int m_linked_cmd_id;

    DataPhysics m_physics;

    FlowUnits m_flowUnit;

    TemperatureUnits m_tempUnit;

    PressureUnits m_pressUnit;

    int m_parameterIndex;



    double m_maxAllowed;

    double m_minAllowed;

    QByteArray m_extraQueryData;



    int m_dataTypeID;

    DataStatus m_status;

    QDateTime m_currentTimeStamp;

    double m_currentData;

    double m_prevData;

    double m_maxData;

    double m_minData;

    QVector<double> m_historyData;

    QVector<QDateTime> m_historyDataTimeStamp;




};

#endif // DEVICEDATA_H
