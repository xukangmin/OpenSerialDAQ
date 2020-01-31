QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    channel.cpp \
    channelwidget.cpp \
    commandwidget.cpp \
    databasemanager.cpp \
    dataprocessor.cpp \
    device.cpp \
    devicedata.cpp \
    devicewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    newchannel.cpp \
    packet.cpp \
    singlechannel.cpp

HEADERS += \
    channel.h \
    channelwidget.h \
    commandwidget.h \
    databasemanager.h \
    dataprocessor.h \
    device.h \
    devicedata.h \
    devicewidget.h \
    mainwindow.h \
    newchannel.h \
    packet.h \
    singlechannel.h

FORMS += \
    channelwidget.ui \
    commandwidget.ui \
    devicewidget.ui \
    mainwindow.ui \
    newchannel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/China-flag-16.png \
    icons/Close-16.png \
    icons/Header16.png \
    icons/TV-16.png \
    icons/Taiwan-flag-16.png \
    icons/bullet-green-16.png \
    icons/bullet-red-16.png \
    icons/close8-black.png \
    icons/close8-red.png \
    icons/connect-24.png \
    icons/cyclic-process-16.png \
    icons/data-sort-16.png \
    icons/document-export-16.png \
    icons/document-import-16.png \
    icons/edit-16.png \
    icons/edit-clear-16.png \
    icons/ethernet-port-16.png \
    icons/exit-16.png \
    icons/help-desk-icon-16.png \
    icons/info-sign-16.png \
    icons/options-16.png \
    icons/play-16.png \
    icons/plug-connect-16.png \
    icons/plug-disconnect-16.png \
    icons/reset-16.png \
    icons/save-16.png \
    icons/serial-pot-16.png \
    icons/text-x-log-16.png \
    icons/tools-16.png \
    icons/usa-flag-16.png

RESOURCES += \
    icons/icons.qrc \
    res.qrc
