#ifndef UNITDAO_H
#define UNITDAO_H


#include <memory>
#include <vector>

class QSqlDatabase;
class Unit;

class UnitDao
{
public:
    UnitDao(QSqlDatabase& database);
    void init() const;

    void addUnit(Unit& ch) const;
    void updateUnit(const Unit& ch) const;
    void removeUnit(int id) const;
    std::unique_ptr<std::vector<std::shared_ptr<Unit>>> units() const;
private:
    QSqlDatabase& mDatabase;
};



#endif // UNITDAO_H
