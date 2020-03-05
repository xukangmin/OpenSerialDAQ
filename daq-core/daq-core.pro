QT       += sql serialport
QT       -= gui

TARGET = daq-core
TEMPLATE = lib
CONFIG += lib c++17

DEFINES += DAQCORE_LIBRARY


SOURCES += \
    Channel/Channel.cpp \
    Channel/ChannelDao.cpp \
    Channel/ChannelModel.cpp \
    Data/Data.cpp \
    Data/DataDao.cpp \
    Data/DataModel.cpp \
    Database/DatabaseManager.cpp \
    Device/Device.cpp \
    Device/DeviceDao.cpp \
    Device/DeviceData.cpp \
    Device/DeviceModel.cpp \
    Generic/GenericDefinition.cpp \
    Generic/GenericDefinitionDao.cpp \
    Models.cpp \
    Thread/ThreadChannel.cpp \
    Thread/ThreadDataProcessor.cpp \
    Packet/Packet.cpp \
    Unit/Unit.cpp \
    Unit/UnitDao.cpp \
    Unit/UnitModel.cpp \
    Variable/Variable.cpp \
    Variable/VariableDao.cpp \
    Variable/VariableModel.cpp

HEADERS += \
    Channel/Channel.h \
    Channel/ChannelDao.h \
    Channel/ChannelModel.h \
    Data/Data.h \
    Data/DataDao.h \
    Data/DataModel.h \
    Database/DatabaseManager.h \
    Device/Device.h \
    Device/DeviceDao.h \
    Device/DeviceData.h \
    Device/DeviceModel.h \
    Generic/GenericDefinition.h \
    Generic/GenericDefinitionDao.h \
    Models.h \
    Thread/ThreadChannel.h \
    Thread/ThreadDataProcessor.h \
    Packet/Packet.h \
    Unit/Unit.h \
    Unit/UnitDao.h \
    Unit/UnitModel.h \
    Variable/Variable.h \
    Variable/VariableDao.h \
    Variable/VariableModel.h \
    daq-core_global.h

RESOURCES += \
    Definition/definitions.qrc

DISTFILES += \
    Definition/protocol.json \
    Definition/units.json
