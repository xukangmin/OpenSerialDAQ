QT       += core gui quick quickwidgets charts quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qml_debug

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
    DialogAddDeviceToChannel.cpp \
    DialogNewDevice.cpp \
    MainWindow.cpp \
    DialogNewChannel.cpp \
    WidgetChannel.cpp \
    WidgetChannelListPage.cpp \
    WidgetCommand.cpp \
    WidgetDevice.cpp \
    WidgetDevicePage.cpp \
    WidgetMiniDevice.cpp \
    WidgetMiniVariable.cpp \
    WidgetSettingPage.cpp \
    WidgetStationPage.cpp \
    WidgetVariable.cpp \
    WidgetVariableChart.cpp \
    main.cpp

HEADERS += \
    DialogAddDeviceToChannel.h \
    DialogNewDevice.h \
    MainWindow.h \
    DialogNewChannel.h \
    WidgetChannel.h \
    WidgetChannelListPage.h \
    WidgetCommand.h \
    WidgetDevice.h \
    WidgetDevicePage.h \
    WidgetMiniDevice.h \
    WidgetMiniVariable.h \
    WidgetSettingPage.h \
    WidgetStationPage.h \
    WidgetVariable.h \
    WidgetVariableChart.h

FORMS += \
    DialogAddDeviceToChannel.ui \
    DialogNewChannel.ui \
    DialogNewDevice.ui \
    WidgetChannel.ui \
    WidgetChannelListPage.ui \
    WidgetCommand.ui \
    WidgetDevice.ui \
    WidgetDevicePage.ui \
    WidgetMiniDevice.ui \
    MainWindow.ui \
    WidgetMiniVariable.ui \
    WidgetSettingPage.ui \
    WidgetStationPage.ui \
    WidgetVariable.ui \
    WidgetVariableChart.ui

RESOURCES += \
    Icons/IconSet.qrc \
    Qml/qdef.qrc

LIBS += -L$$OUT_PWD/../daq-core/ -ldaq-core

INCLUDEPATH += $$PWD/../daq-core
DEPENDPATH += $$PWD/../daq-core

LIBS += -L$$OUT_PWD/../daq-conversion/ -ldaq-conversion

INCLUDEPATH += $$PWD/../daq-conversion
DEPENDPATH += $$PWD/../daq-conversion

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
