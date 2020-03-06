#include "UnitAndConversion.h"

UnitAndConversion::UnitAndConversion()
{
}

UnitAndConversion& UnitAndConversion::instance() {
    static UnitAndConversion singleton;
    return singleton;
}

double UnitAndConversion::unitConvert(double val, QString unit_in, QString unit_out)
{
    return 0;
}

double UnitAndConversion::voscocity(double tempR, QString gas_type)
{
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

