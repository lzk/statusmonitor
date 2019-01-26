#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:43:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tjgd1zsm
TEMPLATE = app

include("../jkinterface/jkinterface.pri")
include(../toec/toec.pri)
include(../common/common.pri)
include("../statusmonitor/statusmonitor.pri")

SOURCES += \
    main.cpp \
        mainwindow.cpp \
    enterpassword.cpp \
    changepassword.cpp \
    checkfingerdialog.cpp \
    fingerhandler.cpp \
    clientthread.cpp \
    watcher.cpp

HEADERS  += mainwindow.h \
    enterpassword.h \
    changepassword.h \
    checkfingerdialog.h \
    fingerhandler.h \
    clientthread.h \
    watcher.h

FORMS    += mainwindow.ui \
    enterpassword.ui \
    changepassword.ui \
    checkfingerdialog.ui

RESOURCES += \
    image.qrc

SOURCES += \
    worker.cpp \
    uinterface.cpp \
    uiconfig.cpp \
    appserver.cpp

HEADERS  += worker.h \
    uinterface.h \
    uiconfig.h \
    appserver.h

CONFIG(debug ,debug|release){
    DEFINES += DEBUG_TO_STDERR
}else{
#contains(QT_ARCH, loongson3a) {
        DEFINES += LOONGSON
#}
}
