#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QString>


class Packet
{
public:
    Packet();

    int m_packet_id;
    QString m_query_str;
    QByteArray m_query_bytes;

    unsigned long timestamp;
    QString node_id;


};

#endif // PACKET_H
