#ifndef THREADCALCULATIONPROCESSOR_H
#define THREADCALCULATIONPROCESSOR_H

#include <QObject>

class ThreadCalculationProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ThreadCalculationProcessor(QObject *parent = nullptr);

signals:

};

#endif // THREADCALCULATIONPROCESSOR_H
