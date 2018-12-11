INCLUDEPATH += $${PWD}

include("../jkinterface/jkinterface.pri")
include("../statusmonitor/statusmonitor.pri")
include(../common/common.pri)
include(../lshell/lshell.pri)

SOURCES += \
    $${PWD}/worker.cpp \
    $${PWD}/uinterface.cpp \
    $${PWD}/uiconfig.cpp

HEADERS  += \
    $${PWD}/worker.h \
    $${PWD}/uinterface.h \
    $${PWD}/uiconfig.h
