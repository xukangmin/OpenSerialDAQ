QT       += sql
QT       -= gui

TARGET = daq-core
TEMPLATE = lib
CONFIG += lib c++11

DEFINES += DAQCORE_LIBRARY


SOURCES += \
    Channel/Channel.cpp \
    Channel/ChannelDao.cpp \
    Channel/ChannelModel.cpp \
    Database/DatabaseManager.cpp \
    Device/Device.cpp \
    Device/DeviceData.cpp \
    Device/DeviceModel.cpp \
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
    Device/DeviceData.h \
    Device/DeviceModel.h \
    Thread/ThreadDataProcessor.h \
    Packet/Packet.h \
    Unit/Unit.h \
    Variable/Variable.h \
    Variable/VariableDao.h \
    Variable/VariableModel.h \
    daq-core_global.h

RESOURCES += \
    Protocol/protocol.qrc
