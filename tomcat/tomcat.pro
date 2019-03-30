#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:43:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tjgd1zsmui
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
    about.cpp \
    statuspaser.cpp

HEADERS  += mainwindow.h \
    enterpassword.h \
    changepassword.h \
    checkfingerdialog.h \
    fingerhandler.h \
    clientthread.h \
    about.h \
    statuspaser.h

FORMS    += mainwindow.ui \
    enterpassword.ui \
    changepassword.ui \
    checkfingerdialog.ui \
    about.ui

RESOURCES += \
    image.qrc

SOURCES += \
    worker.cpp \
    uinterface.cpp \
    uiconfig.cpp \
    appserver.cpp \
    watcher.cpp \
    $${PWD}/error.c \
    $${PWD}/loadresource.c \

HEADERS  += worker.h \
    uinterface.h \
    uiconfig.h \
    appserver.h \
    watcher.h \
    $${PWD}/error.h \
    $${PWD}/ids_string.h \

CONFIG(debug ,debug|release){
    DEFINES += DEBUG_TO_STDERR
    DEFINES += LOONGSON
}else{
    DEFINES += LOONGSON
    DEFINES += RELEASE_release
}

