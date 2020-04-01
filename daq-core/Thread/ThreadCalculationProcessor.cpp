#include "ThreadCalculationProcessor.h"
#include <QMutex>
#include <QDebug>

ThreadCalculationProcessor::ThreadCalculationProcessor(VariableModel& varModel, Variable& var, QHash<QString,QVariant> data) : m_varModel(varModel), m_var(var), m_data(data)
{
}

ThreadCalculationProcessor::~ThreadCalculationProcessor()
{

}

void ThreadCalculationProcessor::run() {

    try {
        m_varModel.calculate(&m_var,m_data);
    } catch(const std::exception& e) {
       qDebug() << " a standard exception was caught, with message '"
                          << e.what() << "'\n";
    }

}
