#ifndef SERIALEMULATOR_H
#define SERIALEMULATOR_H

#include <QByteArray>
#include <QString>
#include <QVector>
#include <memory>

class SerialEmulator
{
public:
    SerialEmulator(QString protocolFileName = ":/protocol.json");

    void loadProtocolFromFile(QString fileName);

    void enableProtocol(QString protocolName);
};

#endif // SERIALEMULATOR_H
