#ifndef VARIABLE_H
#define VARIABLE_H

#include "daq-core_global.h"
#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>
#include <vector>
#include <memory>

static const QString VariableHeaderList[] = {"id","DeviceID","VariableGroupID","Name","Type","Equation","CurrentValue", "Unit","CurrentTimeStamp","DataType","Log"};
static const QString VariableDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","INTEGER", "INTEGER","TEXT","TEXT","TEXT","TEXT","TEXT","DATETIME","TEXT","INTEGER"};
static const int VariableColumnSize = 11;

static const int MAXMEMORYDATASIZE = 1000;

class DAQCORESHARED_EXPORT Variable : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Variable(int id, QHash<QString,QVariant> properties, QVector<QHash<QString,QVariant>> group_properties = QVector<QHash<QString,QVariant>>());

    std::vector<std::shared_ptr<Variable>> required;
    std::vector<std::shared_ptr<Variable>> requiredBy;

    QString equation;

    QVariant currentData;
    QDateTime currentTimeStamp;

    QVector<QVariant> historyData;

    QHash<int,QVariant> toCalculate;

    void addDataToVariable(QHash<QString,QVariant> data, int isInit);
    bool calculate(QHash<QString,QVariant> data);
    double getAverageDataByDataSize(int data_size);
    double getAverageDataByTimePeriod(int seconds);
signals:
    void sendDataToRequiredBy(QHash<QString,QVariant> data);

public slots:
    void getDataFromRequired(QHash<QString,QVariant> data);

};

#endif // VARIABLE_H
