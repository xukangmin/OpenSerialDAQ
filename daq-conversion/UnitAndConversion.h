#ifndef UNITANDCONVERSION_H
#define UNITANDCONVERSION_H

#include "daq-conversion_global.h"
#include <QHash>
#include <QString>
#include <QVector>
#include <QObject>

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

static const QString FunctionNameList[] = {"UnitConv", "Viscocity", "ViscocityCF","MW"};
static const int FunctionNameLength = 4;

class DAQCONVERSION_EXPORT UnitAndConversion : public QObject
{
Q_OBJECT

public:
    Q_INVOKABLE static UnitAndConversion& instance();
    ~UnitAndConversion();

    double unitConvert(double val, QString unit_in, QString unit_out);
    double viscocity(double tempK, QString gas_type);
    double viscocityCF(double tempK, QString gas_type);
    double MW(QString gas_type);
    //QList<double (UnitAndConversion::*) (double, QString,QString)> funList;
    QList<QString> getFunctionNameList();
    double evalSimpleEquation(QString eqn, double val = 0, QString valName = "N/A");
    Q_INVOKABLE QStringList getUnitNameList(QString unitType);
    Q_INVOKABLE QStringList getGasNameList();
    Q_INVOKABLE int getUnitIndexByUnitName(QString unitName);
    Q_INVOKABLE int getGasIndexByName(QString gasTypeName);
protected:
    UnitAndConversion();

private:
    QVector<UnitType> mUnitTypes;
    void loadGasConfig(QString gasConfigPath = ":/gas.json");
    void loadUnitConfig( QString unitConfigPath = ":/units.json");
    QHash<QString,QString> viscosity_equations;
    QHash<QString,QString> viscosity_cf_equations;
    QHash<QString,double> gas_mw;
    QStringList gasTypeList;
};

#endif // UNITANDCONVERSION_H
