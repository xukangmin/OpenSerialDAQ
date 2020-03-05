#ifndef UNITTYPEDAO_H
#define UNITTYPEDAO_H


#include <memory>
#include <vector>

class QSqlDatabase;
class UnitType;

class UnitTypeDao
{
public:
    UnitTypeDao(QSqlDatabase& database);
    void init() const;

    void addUnitType(UnitType& ch) const;
    void updateUnitType(const UnitType& ch) const;
    void removeUnitType(int id) const;
    std::unique_ptr<std::vector<std::shared_ptr<UnitType>>> unittypes() const;
private:
    QSqlDatabase& mDatabase;
};



#endif // UNITTYPEDAO_H
