#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:44:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hornet
TEMPLATE = app

DEFINES += "HORNET=1"

include("../jkinterface/jkinterface.pri")
include("../statusmonitor/statusmonitor.pri")
include(../common/common.pri)


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc

SOURCES += \
    worker.cpp \
    uinterface.cpp \
    uiconfig.cpp

HEADERS  += worker.h \
    uinterface.h \
    uiconfig.h
