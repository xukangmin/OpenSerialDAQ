#ifndef VARIABLE_H
#define VARIABLE_H

#include <QObject>
#include "Unit/Unit.h"
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>


static const QString DataHeaderList[] = {"id","VariableID","TimeStamp","Value","UnitID"};
static const QString DataDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT","TEXT", "INTEGER", "INTEGER"};
static const int DataColumnSize = 5;

class Data : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Data(int id, QHash<QString,QVariant> properties);

};

#endif // VARIABLE_H
