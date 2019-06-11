include("../jkinterface/jkinterface.pri")
include("../statusmonitor/statusmonitor.pri")
include(../common/common.pri)
include(../lshell/lshell.pri)
include(../scanner/scanner.pri)
LIBS += \
    -L$${PWD}/../libs \
    -lusb-1.0 \
    -lcups \
    -llnthr8zcl \

!contains(CONFIG ,static){
LIBS += \
    -ljpeg
}

mac{

    DEFINES += JK_OS_MAC

LIBS += \
    -L/Volumes/work/software/libusb \
    -lnetsnmp \
    -L$${PWD}/../libs/mac \

}else{
    unix{
        contains(QT_ARCH, i386){
                LIBS += \
                    -L$${PWD}/../libs/linux32 \
                    $${PWD}/../libs/linux32/libnetsnmp.a \

        }else{
                LIBS += \
                    -L$${PWD}/../libs/linux64 \
                    $${PWD}/../libs/linux64/libnetsnmp.a \
#                    -lnetsnmp

        }
    }
}

INCLUDEPATH += $${PWD}

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
