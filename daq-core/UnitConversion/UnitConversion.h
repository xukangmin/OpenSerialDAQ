#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>


static const QString UnitConversionHeaderList[] = {"id","SourceUnitID","DestinationUnitID","Equation"};
static const QString UnitConversionDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","INTEGER","INTEGER", "TEXT"};
static const int UnitConversionColumnSize = 4;

class UnitConversion : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    UnitConversion(int id, QHash<QString,QVariant> properties);

};

#endif // UNITCONVERSION_H
