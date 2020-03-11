#ifndef THREADCALCULATIONPROCESSOR_H
#define THREADCALCULATIONPROCESSOR_H

#include <QRunnable>
#include <QObject>

#include <Variable/Variable.h>

class ThreadCalculationProcessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ThreadCalculationProcessor(Variable& var, QHash<QString,QVariant> data);
    ~ThreadCalculationProcessor();

    void run() override;
signals:

private:
    Variable& m_var;
    QHash<QString,QVariant> m_data;
};

#endif // THREADCALCULATIONPROCESSOR_H
