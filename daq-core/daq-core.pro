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
    Database/DatabaseManager.cpp \
    Device/Device.cpp \
    Device/DeviceDao.cpp \
    Device/DeviceData.cpp \
    Device/DeviceModel.cpp \
    Generic/GenericDefinition.cpp \
    Generic/GenericDefinitionDao.cpp \
    Thread/ThreadChannel.cpp \
    Thread/ThreadDataProcessor.cpp \
    Packet/Packet.cpp \
    Unit/Unit.cpp \
    Variable/Variable.cpp \
    Variable/VariableDao.cpp \
    Variable/VariableModel.cpp

HEADERS += \
    Channel/Channel.h \
    Channel/ChannelDao.h \
    Channel/ChannelModel.h \
    Database/DatabaseManager.h \
    Device/Device.h \
    Device/DeviceDao.h \
    Device/DeviceData.h \
    Device/DeviceModel.h \
    Generic/GenericDefinition.h \
    Generic/GenericDefinitionDao.h \
    Thread/ThreadChannel.h \
    Thread/ThreadDataProcessor.h \
    Packet/Packet.h \
    Unit/Unit.h \
    Variable/Variable.h \
    Variable/VariableDao.h \
    Variable/VariableModel.h \
    daq-core_global.h

RESOURCES += \
    Protocol/protocol.qrc
