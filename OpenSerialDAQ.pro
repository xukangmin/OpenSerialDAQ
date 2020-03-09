TEMPLATE = subdirs

SUBDIRS += \
    daq-conversion \
    daq-core \
    daq-desktop

daq-core.depends =  daq-conversion
daq-desktop.depends = daq-core daq-conversion
