#include "VariableGroup.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "Models.h"

VariableGroup::VariableGroup(int id, QHash<QString,QVariant> properties, QVector<QHash<QString,QVariant>> group_properties):
            GenericDefinition(id,properties), m_group_properties(group_properties)
{
}


void VariableGroup::startDAQ(int simulation)
{
    foreach (auto& dev, mDevices) {
       dev->startDAQ(simulation);
    }
}

void VariableGroup::endDAQ(int simulation)
{
    foreach (auto& dev, mDevices) {
       dev->endDAQ(simulation);
    }
}

