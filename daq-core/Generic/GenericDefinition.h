#ifndef GENERICDEFINITION_H
#define GENERICDEFINITION_H

#include <QHash>
#include <QVariant>

class GenericDefinition
{

public:

    GenericDefinition(int id, QHash<QString, QVariant> properties);

    int m_id;

    QHash<QString, QVariant> m_properties;

    QVariant getProperty(QString name) const;

};

#endif
