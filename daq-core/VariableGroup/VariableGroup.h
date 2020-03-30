#ifndef VARIABLEGROUP_H
#define VARIABLEGROUP_H

#include "daq-core_global.h"
#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>
#include <vector>
#include <memory>
#include "Variable/Variable.h"
#include "Device/Device.h"

static const QString VariableGroupHeaderList[] = {"id","Name","InitializationCommands"};
static const QString VariableGroupDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT","TEXT"};
static const int VariableGroupColumnSize = 3;

class DAQCORESHARED_EXPORT VariableGroup : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    VariableGroup(int id, QHash<QString,QVariant> properties, QVector<QHash<QString,QVariant>> group_properties = QVector<QHash<QString,QVariant>>());

    QVector<QHash<QString,QVariant>> m_group_properties;

    std::vector<std::shared_ptr<Variable>> m_variables;

    std::vector<std::shared_ptr<Device>> mDevices;

    void startDAQ(int simulation = 0);
    void endDAQ(int simulation = 0);
};

#endif // VARIABLEGROUP_H
