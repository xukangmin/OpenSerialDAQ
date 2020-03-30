#include "ThreadCalculationProcessor.h"
#include <QMutex>


ThreadCalculationProcessor::ThreadCalculationProcessor(VariableModel& varModel, Variable& var, QHash<QString,QVariant> data) : m_varModel(varModel), m_var(var), m_data(data)
{
}

ThreadCalculationProcessor::~ThreadCalculationProcessor()
{

}

void ThreadCalculationProcessor::run() {

    Models::instance().mutex_global.lock();

    m_varModel.calculate(&m_var,m_data);

    Models::instance().mutex_global.unlock();
}
