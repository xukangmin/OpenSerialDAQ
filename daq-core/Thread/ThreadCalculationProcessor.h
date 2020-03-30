#ifndef THREADCALCULATIONPROCESSOR_H
#define THREADCALCULATIONPROCESSOR_H

#include <QRunnable>
#include <QObject>
#include "Models.h"
#include <Variable/Variable.h>

class ThreadCalculationProcessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ThreadCalculationProcessor(VariableModel& varModel, Variable& var, QHash<QString,QVariant> data);
    ~ThreadCalculationProcessor();

    void run() override;
signals:

private:
    VariableModel& m_varModel;
    Variable& m_var;
    QHash<QString,QVariant> m_data;
};

#endif // THREADCALCULATIONPROCESSOR_H
