#include "UnitAndConversion.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

UnitAndConversion::UnitAndConversion()
{

}



UnitAndConversion::~UnitAndConversion()
{
}

UnitAndConversion& UnitAndConversion::instance() {
    static UnitAndConversion singleton;
    return singleton;
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

            viscosity_equations.insert(singleGasObj["Name"].toString().toUpper(), viscosity_eqn);

            viscosity_cf_eqn.replace("[Viscosity]","(" + viscosity_eqn + ")");

            viscosity_cf_equations.insert(singleGasObj["Name"].toString().toUpper(), viscosity_cf_eqn);
        }
   }
}

void loadUnitConfig( QString unitConfigPath)
{

}

double UnitAndConversion::unitConvert(double val, QString unit_in, QString unit_out)
{
    return 0;
}

double UnitAndConversion::voscocity(double tempR, QString gas_type)
{
    QString eqn = viscosity_equations[gas_type.toUpper()];

    eqn.replace("[IN]",QString::number(tempR));



    return 0;
}

double UnitAndConversion::voscocityCF(double tempR, QString gas_type)
{
    return 0;
}

double UnitAndConversion::MW(QString gas_type)
{
    return 0;
}

