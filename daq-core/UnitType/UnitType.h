#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>


static const QString UnitTypeHeaderList[] = {"id","Name","ConversionType"};
static const QString UnitTypeDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT","TEXT"};
static const int UnitTypeColumnSize = 3;

class UnitType : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    UnitType(int id, QHash<QString,QVariant> properties);

};

#endif // UNITTYPE_H
