#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T13:51:53
#
#-------------------------------------------------

QT       -= core gui

TARGET = tjgd1z
TEMPLATE = lib

DEFINES += TOEC_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include("../jkinterface/jkinterface.pri")
include("../common/common.pri")
unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    $${PWD}/fingermanager.cpp \
    FingerCommon.cpp \
    finger.cpp \
    FingCmd.cpp


HEADERS += \
    $${PWD}/fingermanager.h \
    $${PWD}/filter_check_finger.h \
    FingerCommon.h \
    finger.h \
    FingCmd.h \
    datadefine.h \
    common.h


        DEFINES += LOONGSON
DEFINES += DEBUG_TO_STDERR
#CONFIG(debug ,debug|release){
#    DEFINES += DEBUG_TO_STDERR
#}else{
#    DEFINES += DEBUG_TO_STDERR
##contains(QT_ARCH, loongson3a) {
#        DEFINES += LOONGSON
##}
#}
