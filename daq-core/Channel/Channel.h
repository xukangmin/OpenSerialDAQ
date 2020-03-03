#ifndef CHANNEL_H
#define CHANNEL_H

#include "daq-core_global.h"
#include <QString>
#include <QVariant>
#include "Generic/GenericDefinition.h"
#include <memory>
#include "Device/Device.h"

class ThreadChannel;
class QSerialPort;

enum ChannelType{
    Serial,
    IP
};

static const int BAUDRATE[] = {300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200};

enum SerialBaudRate {
    Baud300 = 300,
    Baud600 = 600,
    Baud1200 = 1200,
    Baud4800 = 4800,
    Baud9600 = 9600,
    Baud14400 = 14400,
    Baud19200 = 19200,
    Baud38400 = 38400,
    Baud57600 = 57600,
    Baud115200 = 115200
};

static const QString ChannelHeaderList[] = {"id","ComPort", "BaudRate", "DataBits", "Parity", "StopBits"};
static const QString ChannelDataType[] = {"INTEGER PRIMARY KEY AUTOINCREMENT","TEXT", "INTEGER", "INTEGER", "TEXT", "INTEGER"};
static const int ChannelColumnSize = 6;

class DAQCORESHARED_EXPORT Channel : public GenericDefinition
{

public:
    Channel(int id, QHash<QString, QVariant> properties);
    ~Channel();

    void startChannel();
    void stopChannel();

    void addDeviceToThread(const std::shared_ptr<Device>& dev);
    void removeDeviceFromThread(const std::shared_ptr<Device>& dev);

private:
    std::unique_ptr<ThreadChannel> threadChannel;

};

#endif // CHANNEL_H
