INCLUDEPATH += $${PWD}
#LIBS += -L$${OUT_PWD}/../statusmonitor -lstatusmonitor

SOURCES += \
    $${PWD}/statusmonitor.cpp \
    $${PWD}/error.c \
    $${PWD}/loadresource.c \
    $$PWD/statusmanager.cpp \
    $$PWD/statusthread.cpp \
    $$PWD/status.cpp \
    $$PWD/cupsmanager.cpp \
    $$PWD/smconfig.cpp


HEADERS += \
    $${PWD}/statusmonitor.h \
    $${PWD}/error.h \
    $${PWD}/ids_string.h \
    $$PWD/statusmanager.h \
    $$PWD/statusthread.h \
    $$PWD/status.h \
    $$PWD/cupsmanager.h \
    $$PWD/smconfig.h
