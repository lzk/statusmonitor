INCLUDEPATH += $${PWD}

include("../jkinterface/jkinterface.pri")
include("../statusmonitor/statusmonitor.pri")
include(../common/common.pri)
include(../lshell/lshell.pri)
include(../scanner/scanner.pri)

SOURCES += \
    $${PWD}/worker.cpp \
    $${PWD}/uinterface.cpp \
    $${PWD}/uiconfig.cpp \
    $$PWD/watcher.cpp \
    $$PWD/statuspaser.cpp

HEADERS  += \
    $${PWD}/worker.h \
    $${PWD}/uinterface.h \
    $${PWD}/uiconfig.h \
    $$PWD/watcher.h \
    $$PWD/statuspaser.h
