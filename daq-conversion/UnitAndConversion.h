#ifndef UNITANDCONVERSION_H
#define UNITANDCONVERSION_H

#include "daq-conversion_global.h"
#include <QString>
#include <QVector>

enum ConversionMethod {
    Factor,
    Equation
};


struct Unit{
    QString name;
    double conversion_factor;
    QVector<QString> equation;
};

struct UnitType{
    QString name;
    QVector<Unit> mUnits;
};


class DAQCONVERSION_EXPORT UnitAndConversion
{
public:
    static UnitAndConversion& instance();
    ~UnitAndConversion();

    double unitConvert(double val, QString unit_in, QString unit_out);
    double voscocity(double tempR, QString gas_type);
    double voscocityCF(double tempR, QString gas_type);
    double MW(QString gas_type);

protected:
    UnitAndConversion();

private:
    QVector<UnitType> mUnitTypes;
};

#endif // UNITANDCONVERSION_H
