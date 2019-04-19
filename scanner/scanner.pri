INCLUDEPATH += $${PWD}
INCLUDEPATH += $${PWD}/../libs/libjpeg

HEADERS += \
    $$PWD/scanner.h \
    $$PWD/scannerapi.h \
    $$PWD/scannerapp.h \
    $$PWD/rawtobmp.h \
    $$PWD/imagetrans.h \
    $$PWD/imageinfo.h \
    $$PWD/ntdcmsapi.h \
    $$PWD/trans_jpg.h

equals(TEMPLATE ,"lib"){
SOURCES += \
    $$PWD/scanner.cpp \
    $$PWD/scannerapi.cpp \
    $$PWD/scannerapp.cpp \
    $$PWD/rawtobmp.cpp \
    $$PWD/imagetrans.cpp \
    $$PWD/ntdcmsapi.cpp \
    $$PWD/trans_jpg.cpp

}

equals(TEMPLATE ,"app"){

    macx{
        LIBS += -L$${PWD}/../libs/mac -llnthr8zcl
    !contains(CONFIG ,static){
    #            LIBS += $${PWD}/../libs/mac/libjpeg.a
        LIBS += -ljpeg
    }
    }else{
        unix{
            contains(QT_ARCH, i386) {
                LIBS += -L$${PWD}/../libs/linux32 -llnthr8zcl
    !contains(CONFIG ,static){
    #            LIBS += $${PWD}/../libs/linux32/libjpeg.a
        LIBS += -ljpeg
    }
            }else{
                LIBS += -L$${PWD}/../libs/linux64 -llnthr8zcl
    !contains(CONFIG ,static){
    #            LIBS += $${PWD}/../libs/linux64/libjpeg.a
        LIBS += -ljpeg
    }
            }
        }
    }
}

