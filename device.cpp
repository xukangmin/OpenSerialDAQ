#include "device.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Device::Device()
{

}


void Device::loadFromConfig(QString protocol_name, QString device_name, int node_id) {
    QFile loadFile(":/protocol/all.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    QByteArray saveData = loadFile.readAll();

    loadFile.close();

    QJsonDocument  loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();

    QString proName, queryStr, regexStr;

    if (json.contains("Protocols")) {
        QJsonObject protocols = json["Protocols"].toObject();
        QJsonArray pArr = json["Protocols"].toArray();
        QJsonObject jb;
        for(int i = 0; i < pArr.size(); i++){
           jb = pArr[i].toObject();

           if (jb.contains("ProtocolName")) {
               if (jb["ProtocolName"].toString() == "LFE") {
                    qDebug() << "test";
               }
           }
        }
    }

    if (json.contains("DataQueryString") && json["DataQueryString"].isString()) {
        queryStr = json["DataQueryString"].toString();
    }

    if (json.contains("DataResponseRegex") && json["DataResponseRegex"].isString()) {
        regexStr = json["DataResponseRegex"].toString();
    }


    qDebug() << json.count();

}
