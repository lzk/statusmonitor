#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:44:56
#
#-------------------------------------------------

include(../statusmonitor.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hornet
TEMPLATE = app

DEFINES += "HORNET=1"

SOURCES += \
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc

