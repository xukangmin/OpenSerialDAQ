#ifndef UNITCONVERSIONDAO_H
#define UNITCONVERSIONDAO_H


#include <memory>
#include <vector>

class QSqlDatabase;
class UnitConversion;

class UnitConversionDao
{
public:
    UnitConversionDao(QSqlDatabase& database);
    void init() const;

    void addUnitConversion(UnitConversion& ch) const;
    void updateUnitConversion(const UnitConversion& ch) const;
    void removeUnitConversion(int id) const;
    std::unique_ptr<std::vector<std::shared_ptr<UnitConversion>>> unitconversions() const;
private:
    QSqlDatabase& mDatabase;
};



#endif // UNITCONVERSIONDAO_H
