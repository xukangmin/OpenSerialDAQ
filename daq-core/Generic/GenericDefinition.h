#ifndef GENERICDEFINITION_H
#define GENERICDEFINITION_H

#include <QHash>
#include <QVariant>
#include "daq-core_global.h"

class DAQCORESHARED_EXPORT GenericDefinition
{

public:

    GenericDefinition(int id, QHash<QString, QVariant> properties);

    int m_id;

    QHash<QString, QVariant> m_properties;

    QVariant getSingleProperty(QString name) const;

    void setSingleProperty(QString key, QVariant property);
};

#endif
