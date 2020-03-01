#include "Generic/GenericDefinition.h"

#include <QDebug>

GenericDefinition::GenericDefinition(int id, QHash<QString, QVariant> properties) :
                m_id(id),
                m_properties(properties)
{

}

QVariant GenericDefinition::getProperty(QString name) const {
    return m_properties[name];
}
