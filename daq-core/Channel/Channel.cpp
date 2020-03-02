#include "Channel.h"
#include "Thread/ThreadChannel.h"

#include <QDebug>

Channel::Channel(int id, QHash<QString, QVariant> properties) :
         GenericDefinition(id,properties),
         threadChannel(new ThreadChannel(*this))
{
}

Channel::~Channel() {
    threadChannel->stopChannel();
    threadChannel->wait();
    qDebug() << "channel " << m_id << " destroyed!";
}


void Channel::startChannel()
{
    threadChannel->startChannel();
}

void Channel::stopChannel()
{
    threadChannel->stopChannel();
}
