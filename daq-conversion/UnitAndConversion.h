#ifndef UNITANDCONVERSION_H
#define UNITANDCONVERSION_H

#include "daq-conversion_global.h"
#include <QHash>
#include <QString>
#include <QVector>
#include "exprtk.hpp"

enum ConversionMethod {
    Factor,
    Equation
};


struct Unit{
    QString name;
    double conversion_factor;
    int index;
    QVector<QString> equation;
    QVector<QString> alias;
};

struct UnitType{
    QString name;
    QString conversion_type;
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
    void loadGasConfig(QString gasConfigPath = ":/gas.json");
    void loadUnitConfig( QString unitConfigPath = ":/units.json");
    double evalSimpleEquation(double val, QString eqn, QString valName = "");
    QHash<QString,QString> viscosity_equations;
    QHash<QString,QString> viscosity_cf_equations;
    QHash<QString,double> gas_mw;
};

#endif // UNITANDCONVERSION_H
