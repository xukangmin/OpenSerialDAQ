#include "Variable.h"

Variable::Variable(QObject *parent) : QObject(parent)
{

}

Variable::Variable(int id, QString name, QString unit, QString equation):
        m_id(id),
        m_name(name),
        m_unitStr(unit),
        m_equation(equation)
{

}
