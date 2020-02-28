TEMPLATE = subdirs

SUBDIRS += \
    daq-core \
    daq-desktop

daq-desktop.depends = daq-core
