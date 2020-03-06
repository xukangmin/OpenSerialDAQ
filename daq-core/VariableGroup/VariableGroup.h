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

static const QString VariableGroupHeaderList[] = {"id","Name"};
static const QString VariableGroupDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT"};
static const int VariableGroupColumnSize = 2;

class DAQCORESHARED_EXPORT VariableGroup : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    VariableGroup(int id, QHash<QString,QVariant> properties, QHash<QString,QVariant> group_properties = QHash<QString,QVariant>());

    QHash<QString,QVariant> m_group_properties;

    std::vector<std::shared_ptr<Variable>> m_variables;
};

#endif // VARIABLEGROUP_H
