#ifndef NEWCHANNEL_H
#define NEWCHANNEL_H

#include <QDialog>
#include <QSerialPortInfo>
#include "channel.h"

namespace Ui {
class NewChannel;
}

class NewChannel : public QDialog
{
    Q_OBJECT

public:
    explicit NewChannel(QWidget *parent = nullptr);
    ~NewChannel();

    void getAvailablePorts();

    Channel getChannelInfo();

    QList<QString> m_portLists;

private:
    Ui::NewChannel *ui;
};

#endif // NEWCHANNEL_H
