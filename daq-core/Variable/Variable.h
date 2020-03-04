#ifndef VARIABLE_H
#define VARIABLE_H

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

class Variable : public QObject, public GenericDefinition
{
    Q_OBJECT
public:
    Variable(int id, QHash<QString,QVariant> properties);

    QDateTime m_currentTimeStamp;

    double m_currentData;

    double m_prevData;

    double m_maxData;

    double m_minData;

    QVector<double> m_historyData;

    QVector<QDateTime> m_historyDataTimeStamp;

};

#endif // VARIABLE_H
