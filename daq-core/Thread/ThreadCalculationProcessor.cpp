#include "ThreadCalculationProcessor.h"
#include <QMutex>
#include "Models.h"

ThreadCalculationProcessor::ThreadCalculationProcessor(Variable& var, QHash<QString,QVariant> data) : m_var(var), m_data(data)
{
}

ThreadCalculationProcessor::~ThreadCalculationProcessor()
{

}

void ThreadCalculationProcessor::run() {

    Models::instance().mutex_global.lock();

    m_var.calculate(m_data);

    Models::instance().mutex_global.unlock();
}
