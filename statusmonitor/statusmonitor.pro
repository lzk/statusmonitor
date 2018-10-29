#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:47:29
#
#-------------------------------------------------

QT       -= core gui

TARGET = statusmonitor
TEMPLATE = lib
#CONFIG += staticlib

SOURCES += statusmonitor.cpp \
    usbio.cpp \
    usbdevice.cpp \
    usbapi_libusb.cpp \
    testlibusb.c \
    api_libcups.cpp \
    log.cpp \
    finger.cpp \
    trans.cpp \
    fingermanager.cpp \
    error.c \
    loadresource.c


HEADERS += statusmonitor.h \
    usbio.h \
    usbdevice.h \
    usbapi_libusb.h \
    api_libcups.h \
    log.h \
    finger.h \
    trans.h \
    fingermanager.h \
    filter_check_finger.h \
    error.h \
    ids_string.h

INCLUDEPATH += ../../../../../../workspace/3rdparty/libusb-1.0.22/libusb \
                            ../../../../../../workspace/linux/cups-2.0.2

unix:!macx: LIBS += -L$${PWD} -lusb-1.0  -lcups

macx: LIBS += -L/Volumes/work/software/libusb -lusb-1.0 -lcups
