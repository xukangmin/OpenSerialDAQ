#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>
#include <QVector>

class Packet
{
public:
    Packet();

    int m_packet_id;
    QString m_query_str;
    QByteArray m_query_bytes;
    unsigned long timestamp;
    QString device_id;
    QString m_parse_regex;
    QVector<double> result;


};

#endif // PACKET_H
