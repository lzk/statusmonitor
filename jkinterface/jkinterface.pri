
INCLUDEPATH += \
            $${PWD} \
            $$PWD/../libs \
            $$PWD/../libs/cups-2.2.8

HEADERS += \
    $${PWD}/usbapi_libusb.h \
    $${PWD}/api_libcups.h \
    $${PWD}/log.h \
    $${PWD}/trans.h \
    $$PWD/jkinterface.h \
    $$PWD/deviceio.h \
    $$PWD/jkconfig.h \
    $$PWD/filelocker.h

SOURCES += \
    $${PWD}/usbapi_libusb.cpp \
    $${PWD}/testlibusb.c \
    $${PWD}/api_libcups.cpp \
    $${PWD}/log.cpp \
    $${PWD}/trans.cpp \
    $$PWD/jkconfig.cpp \
    $$PWD/deviceio.cpp \
    $$PWD/filelocker.cpp

macx:{
    DEFINES += JK_OS_MAC
}
