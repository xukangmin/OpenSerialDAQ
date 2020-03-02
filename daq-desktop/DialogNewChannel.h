#ifndef NEWCHANNEL_H
#define NEWCHANNEL_H

#include <QDialog>
#include "Channel/Channel.h"
#include "Thread/ThreadChannel.h"

namespace Ui {
class DialogNewChannel;
}

class DialogNewChannel : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewChannel(QWidget *parent = nullptr);
    ~DialogNewChannel();

    void getAvailablePorts();

    QHash<QString, QVariant> getChannelInfo();

    QList<QString> m_portLists;

private:
    Ui::DialogNewChannel *ui;
};

#endif // NEWCHANNEL_H
