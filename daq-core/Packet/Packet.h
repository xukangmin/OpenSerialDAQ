#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>
#include <QVector>
#include "Device/Device.h"
#include <memory>

class Packet
{
public:
    Packet(const std::shared_ptr<Device>& dev);

    int m_packet_id;
    QString m_query_str;
    QByteArray m_query_bytes;
    QByteArray m_response_bytes;
    unsigned long time_stamp;
    QString device_id;
    QString m_parse_regex;
    QVector<double> result;
    int m_cmd_id;
    const std::shared_ptr<Device>& m_dev;
};

#endif // PACKET_H
