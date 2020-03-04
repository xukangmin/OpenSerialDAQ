#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMetaType>
#include <limits>
#include <Generic/GenericDefinition.h>


static const QString UnitHeaderList[] = {"id","Name","Conversion","TypeID"};
static const QString UnitDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT","Real", "INTEGER"};
static const int UnitColumnSize = 4;

class Unit : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Unit(int id, QHash<QString,QVariant> properties);

};

#endif // UNIT_H
