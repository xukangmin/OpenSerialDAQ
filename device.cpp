#include "device.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>

Device::Device(int node_id):
        m_node_id(node_id)
{
    m_device_id = 1;
}

Device::Device(int node_id, QString protocol_name):
        m_node_id(node_id)
{
    loadFromConfig(protocol_name);
}


QByteArray Device::buildQueryCmd(QString cmdName) {
    bool parse_ok;
    int index = 0;
    for(int i = 0; i < m_commands.size(); i++) {
        if (cmdName == m_commands[i].name) {
            index = i;
            break;
        }
    }

    Command &cmd = m_commands[index];

    cmd.query_cmd.clear();

    foreach(QString cmd_str, cmd.query_cmd_arr) {
        QRegularExpression reg_to_resolve("\\[\\w+\\]");
        QRegularExpression reg_hex("0x[0-9a-fA-F]{2}");

        if (reg_to_resolve.match(cmd_str).hasMatch()) {

            if (cmd_str.contains("Api")) {
                // generate random api number
                cmd.api = rand() % 200;
                cmd.query_cmd.append(cmd.api);
            }
            else if (cmd_str.contains("Header")) {
                cmd.query_cmd.append(cmd.header);
            }
            else if (cmd_str.contains("Node")) {
                cmd.query_cmd.append(m_node_id);
            }
            else if (cmd_str.contains("Cmd")) {
                cmd.query_cmd.append(cmd.cmd);
            }
            else if (cmd_str.contains("Length")) {
                cmd.query_cmd.append(cmd.length);
            }
            else if (cmd_str.contains("CheckSum")) {
                if (cmd.check_sum_rule == "Simple") {
                    // calculate check sum
                    if (cmd.query_cmd.at(1) == cmd.query_cmd.size() - 1 ) { // size ok
                        uint32_t total_cr = 0;
                        for (int cr_i = 2; cr_i < cmd.query_cmd.size(); cr_i++) {
                            total_cr += cmd.query_cmd[cr_i];
                            total_cr = 0xFF - (total_cr & 0xFF);
                        }

                        cmd.query_cmd.append(total_cr);
                    }
                }
            }

        } else if (reg_hex.match(cmd_str).hasMatch()) {
            // convert to hex
            cmd.query_cmd.append(cmd_str.toUInt(&parse_ok,16));
        } else {

        }
    }

    return m_commands[index].query_cmd;
}


void Device::parseRxData(QByteArray rx_data, int cmd_id) {

    Command cmd = m_commands[cmd_id];

    if (cmd.parse_method == "Exact") {

        bool isValidated = true;

        foreach(DataFormat fm, cmd.data_formats) {
            if (fm.usage == "Validation"){
                if (fm.name == "Header") {
                    if (!((uint8_t)rx_data.at(fm.location) == cmd.header)) {
                        isValidated = false;
                    }
                }
                else if (fm.name == "Length") {
                    if (!(rx_data.at(fm.location) == rx_data.size() - 2)) {
                        isValidated = false;
                    }
                }
                else if (fm.name == "Api") {
                    if (!((uint8_t)rx_data.at(fm.location) == cmd.api)) {
                        isValidated = false;
                    }
                }
                else if (fm.name == "Node") {
                    if (!(rx_data.at(fm.location) == m_node_id)) {
                        isValidated = false;
                    }
                }
                else if (fm.name == "Cmd") {
                    if (!((uint8_t)rx_data.at(fm.location) == cmd.cmd + 1)) {
                        isValidated = false;
                    }
                }


            }
            else if (fm.usage == "Data") {
                // save data to memory & db
                if (fm.type == "float") {
                    float data = 0;

                    uint8_t tmp_float[sizeof(float)];

                    for(int i = 0; i < sizeof(float);i++) {
                        tmp_float[i] = rx_data.at(fm.location + i);
                    }

                    memcpy(&data, tmp_float, sizeof(float));
                    qDebug() << data << " " << fm.name;
                }
            }
        }
    }
}



void Device::loadFromConfig(QString protocol_name) {
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
        QJsonObject singleProtocol;
        for(int i = 0; i < pArr.size(); i++){
           singleProtocol = pArr[i].toObject();

           if (singleProtocol.contains("ProtocolName")) {
               if (singleProtocol["ProtocolName"].toString() == protocol_name) {
                    QJsonArray cmdArr = singleProtocol["CmdLists"].toArray();
                    for(int j = 0; j < cmdArr.size(); j++) {
                        Command cmd;
                        bool parse_ok;
                        QJsonObject singleCmd = cmdArr[j].toObject();
                        if (singleCmd.contains("Name") && singleCmd["Name"].isString()) {
                            cmd.name = singleCmd["Name"].toString();
                        }

                        if (singleCmd.contains("Description") && singleCmd["Description"].isString()) {
                            cmd.description = singleCmd["Description"].toString();
                        }

                        if (singleCmd.contains("Header") && singleCmd["Header"].isString()) {
                            cmd.header = singleCmd["Header"].toString().toUInt(&parse_ok,16);
                        }

                        if (singleCmd.contains("Cmd") && singleCmd["Cmd"].isString()) {
                            cmd.cmd = singleCmd["Cmd"].toString().toUInt(&parse_ok,16);
                        }

                        if (singleCmd.contains("Length") && singleCmd["Length"].isString()) {
                            cmd.length = singleCmd["Length"].toString().toUInt(&parse_ok,16);
                        }

                        if (singleCmd.contains("ParseMethod") && singleCmd["ParseMethod"].isString()) {
                            cmd.parse_method = singleCmd["ParseMethod"].toString();
                        }

                        if (singleCmd.contains("DAQ") && singleCmd["DAQ"].isDouble()) {
                            if (singleCmd["DAQ"].toInt() == 1) {
                                cmd.isDAQ = true;
                            } else {
                                cmd.isDAQ = false;
                            }
                        }

                        if (singleCmd.contains("CheckSumRule") && singleCmd["CheckSumRule"].isString())
                        {
                            cmd.check_sum_rule = singleCmd["CheckSumRule"].toString();
                        }


                        if (singleCmd.contains("DataFormat") && singleCmd["DataFormat"].isArray()) {
                            QJsonArray data_format_arr = singleCmd["DataFormat"].toArray();
                            for (int k = 0; k < data_format_arr.size(); k++) {
                                QJsonObject singleDataFormat = data_format_arr[k].toObject();
                                DataFormat data_format;
                                if (singleDataFormat.contains("Name") && singleDataFormat["Name"].isString()) {
                                    data_format.name = singleDataFormat["Name"].toString();
                                }

                                if (singleDataFormat.contains("Type") && singleDataFormat["Type"].isString()) {
                                    data_format.type = singleDataFormat["Type"].toString();
                                }

                                if (singleDataFormat.contains("Location") && singleDataFormat["Location"].isDouble()) {
                                    data_format.location = singleDataFormat["Location"].toInt();
                                }

                                if (singleDataFormat.contains("Unit") && singleDataFormat["Unit"].isString()) {
                                    data_format.unit = singleDataFormat["Unit"].toString();
                                }

                                if (singleDataFormat.contains("Usage") && singleDataFormat["Usage"].isString()) {
                                    data_format.usage = singleDataFormat["Usage"].toString();
                                }

                                cmd.data_formats.append(data_format);
                            }
                        }


                        if (singleCmd.contains("QueryCmdList") && singleCmd["QueryCmdList"].isArray()) {
                            QJsonArray queryCmdArr = singleCmd["QueryCmdList"].toArray();

                            for(int k = 0; k < queryCmdArr.size(); k++) {
                                cmd.query_cmd_arr.append(queryCmdArr[k].toString());
                            }
                        }


                        m_commands.append(cmd);
                    }
               }
           }
        }
    }

    buildQueryCmd("ReadData");

    qDebug() << m_commands.size();

}
