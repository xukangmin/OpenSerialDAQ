#ifndef VARIABLE_H
#define VARIABLE_H

#include "daq-core_global.h"
#include <QObject>
#include "Unit/Unit.h"
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>


static const QString VariableHeaderList[] = {"id","Name","Equation","DeviceID","UnitID"};
static const QString VariableDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT","TEXT", "INTEGER", "INTEGER"};
static const int VariableColumnSize = 5;

class DAQCORESHARED_EXPORT Variable : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Variable(int id, QHash<QString,QVariant> properties);

};

#endif // VARIABLE_H
