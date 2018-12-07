#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:43:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tomcat
TEMPLATE = app

DEFINES += "TOMCAT=1"

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
    clientthread.cpp

HEADERS  += mainwindow.h \
    enterpassword.h \
    changepassword.h \
    checkfingerdialog.h \
    fingerhandler.h \
    clientthread.h

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
