#ifndef VARIABLEDAO_H
#define VARIABLEDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Variable;

class VariableDao
{
public:
    VariableDao(QSqlDatabase& database);
    void init() const;

    void addVariable(Variable& ch) const;
    void updateVariable(const Variable& ch) const;
    void removeVariable(int id) const;
    std::unique_ptr<std::vector<std::shared_ptr<Variable>>> variables() const;
private:
    QSqlDatabase& mDatabase;
};



#endif // VARIABLEDAO_H
