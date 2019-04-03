INCLUDEPATH += $${PWD}
#LIBS += -L$${OUT_PWD}/../statusmonitor -lstatusmonitor

SOURCES += \
    $${PWD}/statusmonitor.cpp \
    $$PWD/statusmanager.cpp \
    $$PWD/statusthread.cpp \
    $$PWD/status.cpp \
    $$PWD/cupsmanager.cpp \
    $$PWD/smconfig.cpp \
    $$PWD/statuswatcher.cpp


HEADERS += \
    $${PWD}/statusmonitor.h \
    $$PWD/statusmanager.h \
    $$PWD/statusthread.h \
    $$PWD/status.h \
    $$PWD/cupsmanager.h \
    $$PWD/smconfig.h \
    $$PWD/statuswatcher.h
