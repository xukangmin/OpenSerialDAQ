TEMPLATE = subdirs

SUBDIRS += \
    daq-conversion \
    daq-core \
    daq-desktop

daq-desktop.depends = daq-core
