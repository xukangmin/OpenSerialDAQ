QT       += core gui quickwidgets

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
    MainWindow.cpp \
    DialogAddDeviceToChannel.cpp \
    DialogNewChannel.cpp \
    WidgetChannel.cpp \
    WidgetCommand.cpp \
    WidgetDevice.cpp \
    WidgetMiniDevice.cpp \
    main.cpp

HEADERS += \
    MainWindow.h \
    DialogAddDeviceToChannel.h \
    DialogNewChannel.h \
    WidgetChannel.h \
    WidgetCommand.h \
    WidgetDevice.h \
    WidgetMiniDevice.h

FORMS += \
    DialogAddDeviceToChannel.ui \
    DialogNewChannel.ui \
    WidgetChannel.ui \
    WidgetCommand.ui \
    WidgetDevice.ui \
    WidgetMiniDevice.ui \
    MainWindow.ui

RESOURCES += \
    Icons/icons.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../daq-core/release/ -lgallery-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../daq-core/debug/ -lgallery-core
else:unix: LIBS += -L$$OUT_PWD/../daq-core/ -ldaq-core

INCLUDEPATH += $$PWD/../daq-core
DEPENDPATH += $$PWD/../daq-core

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
