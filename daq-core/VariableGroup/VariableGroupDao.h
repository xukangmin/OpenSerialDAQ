#ifndef VARIABLEGROUPDAO_H
#define VARIABLEGROUPDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class VariableGroup;
class QMutex;

class VariableGroupDao
{
public:
    VariableGroupDao(QSqlDatabase& database, QMutex& dbMutex);
    void init() const;

    void addVariableGroup(VariableGroup& ch) const;
    void updateVariableGroup(const VariableGroup& ch) const;
    void removeVariableGroup(int id) const;
    void removeAll() const;
    std::unique_ptr<std::vector<std::shared_ptr<VariableGroup>>> variablegroups() const;
private:
    QSqlDatabase& mDatabase;
    QMutex& mMutex;
};



#endif // VARIABLEGROUPDAO_H
