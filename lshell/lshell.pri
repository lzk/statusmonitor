INCLUDEPATH += $${PWD}

SOURCES += \
    $${PWD}/lshell.cpp \


HEADERS += \
    $${PWD}/lshell.h \
    $${PWD}/lshellstruct.h \

equals(TEMPLATE ,"lib"){
SOURCES += \
    $${PWD}/lshell.cpp \

}

equals(TEMPLATE ,"app"){

}

