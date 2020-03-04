#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "Unit/Unit.h"
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>


static const QString DataHeaderList[] = {"id","VariableID","TimeStamp","Value"};
static const QString DataDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","INTEGER","DATETIME DEFAULT CURRENT_TIMESTAMP", "DOUBLE"};
static const int DataColumnSize = 4;

class Data : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Data(int id, QHash<QString,QVariant> properties);

};

#endif // DATA_H
