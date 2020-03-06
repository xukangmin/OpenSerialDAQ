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

static const QString VariableHeaderList[] = {"id","DeviceID","VariableGroupID","Name","Type","Equation","Unit","CurrentValue","CurrentTimeStamp","DataType","Log"};
static const QString VariableDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","INTEGER", "INTEGER","TEXT","TEXT","TEXT","TEXT","TEXT","DATETIME","TEXT","INTEGER"};
static const int VariableColumnSize = 11;

static const int MAXMEMORYDATASIZE = 1000;

class DAQCORESHARED_EXPORT Variable : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Variable(int id, QHash<QString,QVariant> properties, QHash<QString,QVariant> group_properties = QHash<QString,QVariant>());

    std::vector<std::shared_ptr<Variable>> required;
    std::vector<std::shared_ptr<Variable>> requiredBy;

    QString equation;

    QVariant currentData;
    QDateTime currentTimeStamp;

    std::vector<QVariant> historyData;

    QHash<int,QVariant> toCalculate;

    void addDataToVariable(QVariant data);

signals:
    void sendDataToRequiredBy(int var_id, QVariant data);

public slots:
    void getDataFromRequired(int var_id, QVariant data);

};

#endif // VARIABLE_H
