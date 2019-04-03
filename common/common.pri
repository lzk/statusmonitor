INCLUDEPATH += $${PWD}

INCLUDEPATH += ../libs ../libs/cups-2.2.8

LIBS += -L$${PWD}/../libs -lusb-1.0  -lcups
mac{
    LIBS += -lnetsnmp
}else{
    unix{
        contains(QT_ARCH, i386) {
            LIBS += $${PWD}/../libs/linux32/libnetsnmp.a
        }else{
            LIBS += $${PWD}/../libs/linux64/libnetsnmp.a
        }
    }
}

macx: LIBS += -L/Volumes/work/software/libusb

QT       += core network

HEADERS += \
    $$PWD/serverthread.h \
    $$PWD/commonapi.h \
    $$PWD/usbio.h \
    $$PWD/netio.h \
    $$PWD/devicemanager.h

SOURCES += \
    $$PWD/serverthread.cpp \
    $$PWD/commonapi.cpp \
    $$PWD/usbio.cpp \
    $$PWD/netio.cpp \
    $$PWD/devicemanager.cpp \
    $$PWD/snmpapi.cpp


mac{

    SOURCES += \
    $$PWD/macapi.cpp

}else{

    SOURCES += \
    $$PWD/linuxapi.cpp

}
