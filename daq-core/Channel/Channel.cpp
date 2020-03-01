#include "Channel.h"
#include "Thread/ThreadChannel.h"

#include <QDebug>

Channel::Channel(int id, QHash<QString, QVariant> properties) :
         GenericDefinition(id,properties)
{
}
