#-------------------------------------------------
#
# Project created by QtCreator 2018-07-19T13:43:32
#
#-------------------------------------------------

include(../statusmonitor.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tomcat
TEMPLATE = app

DEFINES += "TOMCAT=1"

SOURCES += \
        mainwindow.cpp \
    enterpassword.cpp \
    changepassword.cpp \
    checkfingerdialog.cpp

HEADERS  += mainwindow.h \
    enterpassword.h \
    changepassword.h \
    checkfingerdialog.h

FORMS    += mainwindow.ui \
    enterpassword.ui \
    changepassword.ui \
    checkfingerdialog.ui

RESOURCES += \
    image.qrc

