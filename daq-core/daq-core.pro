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
    Variable/Variable.cpp \
    Variable/VariableDao.cpp \
    Variable/VariableModel.cpp \
    VariableGroup/VariableGroup.cpp \
    VariableGroup/VariableGroupDao.cpp \
    VariableGroup/VariableGroupModel.cpp

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
    Variable/Variable.h \
    Variable/VariableDao.h \
    Variable/VariableModel.h \
    VariableGroup/VariableGroup.h \
    VariableGroup/VariableGroupDao.h \
    VariableGroup/VariableGroupModel.h \
    daq-core_global.h

RESOURCES += \
    Definition/definitions.qrc

DISTFILES += \
    Definition/EquationLaminar.json \
    Definition/protocol.json \
    Definition/stationconfig.json \
    Definition/units.json

LIBS += -L$$OUT_PWD/../daq-conversion/ -ldaq-conversion

INCLUDEPATH += $$PWD/../daq-conversion
DEPENDPATH += $$PWD/../daq-conversion
