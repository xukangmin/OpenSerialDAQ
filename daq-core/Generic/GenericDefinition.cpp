#include "Generic/GenericDefinition.h"

#include <QDebug>

GenericDefinition::GenericDefinition(int id, QHash<QString, QVariant> properties) :
                m_id(id),
                m_properties(properties)
{

}

QVariant GenericDefinition::getSingleProperty(QString name) const {
    return m_properties[name];
}

void GenericDefinition::setSingleProperty(QString key, QVariant property) {
    m_properties[key] = property;
}
