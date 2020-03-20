#include "UnitAndConversion.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "tinyexpr.h"

UnitAndConversion::UnitAndConversion()
{
    loadGasConfig();
    loadUnitConfig();
}



UnitAndConversion::~UnitAndConversion()
{
}

UnitAndConversion& UnitAndConversion::instance() {
    static UnitAndConversion singleton;
    return singleton;
}

int UnitAndConversion::getUnitIndexByUnitName(QString unitName) {
    QStringList tmp;

    foreach(auto tp, mUnitTypes)
    {
        for(int i = 0; i < tp.mUnits.size(); i++) {
            if (tp.mUnits.at(i).name.toUpper() == unitName.toUpper()) {
                return i;
            }
        }
    }

    return 0;

}

QStringList UnitAndConversion::getUnitNameList(QString unitType) {
    QStringList tmp;

    foreach(auto tp, mUnitTypes)
    {
        if (unitType.toUpper() == tp.name.toUpper()) {
            foreach(auto unit, tp.mUnits){
                tmp << unit.name;
            }
        }
    }

    return tmp;
}


QList<QString> UnitAndConversion::getFunctionNameList() {
    QList<QString> tmp;
    for(int i = 0; i < FunctionNameLength; i++) {
        tmp.append(FunctionNameList[i]);
    }

//    funList.append(&UnitAndConversion::unitConvert);

    return tmp;

}

void UnitAndConversion::loadGasConfig(QString gasConfigPath)
{
    QFile loadFile(gasConfigPath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
           qWarning("Couldn't open save file.");
    }

   QByteArray dataRead = loadFile.readAll();

   loadFile.close();

   QJsonDocument loadDoc(QJsonDocument::fromJson(dataRead));


   QJsonObject json = loadDoc.object();

   if (json.contains("Gas"))
   {
        QJsonArray pArr = json["Gas"].toArray();

        for(int i = 0; i < pArr.size(); i++) {
            QJsonObject singleGasObj = pArr[i].toObject();

            QString viscosity_eqn = singleGasObj["Viscosity"].toString();
            QString viscosity_cf_eqn = singleGasObj["ViscosityCF"].toString();

            double gas_mw_value = singleGasObj["MW"].toDouble();

            viscosity_equations.insert(singleGasObj["Name"].toString().toUpper(), viscosity_eqn);

            viscosity_cf_eqn.replace("Viscosity","(" + viscosity_eqn + ")");

            viscosity_cf_equations.insert(singleGasObj["Name"].toString().toUpper(), viscosity_cf_eqn);

            gas_mw.insert(singleGasObj["Name"].toString().toUpper(), gas_mw_value);
        }
   }
}

void UnitAndConversion::loadUnitConfig( QString unitConfigPath)
{
    QFile loadFile(unitConfigPath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
           qWarning("Couldn't open save file.");
    }

   QByteArray dataRead = loadFile.readAll();

   loadFile.close();

   QJsonDocument loadDoc(QJsonDocument::fromJson(dataRead));


   QJsonObject json = loadDoc.object();

   if (json.contains("Units"))
   {
        QJsonArray pArr = json["Units"].toArray();

        for(int i = 0; i < pArr.size(); i++) {
            QJsonObject singleUnitTypeObj = pArr[i].toObject();

            UnitType unitType;

            unitType.name = singleUnitTypeObj["Type"].toString();
            unitType.conversion_type = singleUnitTypeObj["ConversionType"].toString();
           if (singleUnitTypeObj.contains("UnitList")) {
               QJsonArray dArr = singleUnitTypeObj["UnitList"].toArray();

               for(int j = 0; j < dArr.size(); j++) {
                    QJsonObject singleUnitObj = dArr[j].toObject();

                    Unit unit;

                    unit.name = singleUnitObj["Name"].toString();
                    unit.index = j;
                    if (singleUnitTypeObj["ConversionType"].toString() == "Factor") {
                        unit.conversion_factor = singleUnitObj["Conversion"].toDouble();
                    }
                    else if (singleUnitTypeObj["ConversionType"].toString() == "Equation") {
                        QJsonArray singleConvArr = singleUnitObj["Conversion"].toArray();

                        for (int k = 0; k < singleConvArr.size(); k++) {
                            unit.equation.append(singleConvArr[k].toString());
                        }
                    }

                    if (singleUnitObj.contains("Alias")) {
                        QJsonArray singleAliasArr = singleUnitObj["Alias"].toArray();
                        for (int k = 0; k < singleAliasArr.size(); k++) {
                            unit.alias.append(singleAliasArr[k].toString());
                        }
                    }

                    unitType.mUnits.append(unit);
               }

           }

           mUnitTypes.append(unitType);

        }


   }
}

double UnitAndConversion::unitConvert(double val, QString unit_in, QString unit_out)
{
    // first find unit in and unit out type
    if (unit_in.toUpper() == unit_out.toUpper()) {
        return val;
    }

    Unit unit_in_def;

    Unit unit_out_def;

    QString unit_in_type = "";

    QString unit_out_type = "";

    QString unit_conv_type = "";

    foreach (auto unitType, mUnitTypes) {
        foreach(auto unit, unitType.mUnits) {
            if (unit_in.toUpper() == unit.name.toUpper() || unit.alias.contains(unit_in)) {
                unit_in_type = unitType.name;
                unit_conv_type = unitType.conversion_type;
                unit_in_def = unit;
            }

            if (unit_out.toUpper() == unit.name.toUpper() || unit.alias.contains(unit_out)) {
                unit_out_type = unitType.name;
                unit_out_def = unit;
            }
        }
    }

    if (unit_in_type == unit_out_type) {  // type matched
        if (unit_conv_type == "Factor") {
            return val * unit_out_def.conversion_factor / unit_in_def.conversion_factor;
        }
        else if (unit_conv_type == "Equation")
        {
            return evalSimpleEquation(unit_in_def.equation.at(unit_out_def.index), val, "x");
        }
    }


    return 0;
}

double UnitAndConversion::evalSimpleEquation(QString eqn, double val, QString valName)
{

    eqn.replace(valName,QString::number(val));

    double ret = 0;

    int err;

    ret = te_interp(eqn.toStdString().c_str(), &err);


    return ret;
}

double UnitAndConversion::voscocity(double tempR, QString gas_type)
{

    QString eqn = viscosity_equations[gas_type.toUpper()];

    return evalSimpleEquation(eqn,tempR,"x");
}

double UnitAndConversion::voscocityCF(double tempR, QString gas_type)
{


    QString eqn = viscosity_cf_equations[gas_type.toUpper()];

    return evalSimpleEquation(eqn,tempR,"x");
}

double UnitAndConversion::MW(QString gas_type)
{

    return gas_mw[gas_type.toUpper()];
}

