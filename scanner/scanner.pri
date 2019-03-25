INCLUDEPATH += $${PWD}

macx{
    LIBS += -L$${PWD}/../libs/mac -llnthr8zcl
            LIBS += $${PWD}/../libs/mac/libjpeg.a
}else{
    unix{
        contains(QT_ARCH, i386) {
            LIBS += -L$${PWD}/../libs/linux32 -llnthr8zcl
            LIBS += $${PWD}/../libs/linux32/libjpeg.a
        }else{
            LIBS += -L$${PWD}/../libs/linux64 -llnthr8zcl
            LIBS += $${PWD}/../libs/linux64/libjpeg.a
        }
    }
}
#LIBS += -ljpeg

SOURCES += \
    $$PWD/scanner.cpp \
    $$PWD/scannerapi.cpp \
    $$PWD/scannerapp.cpp \
    $$PWD/rawtobmp.cpp \
    $$PWD/imagetrans.cpp \
    $$PWD/ntdcmsapi.cpp \
    $$PWD/trans_jpg.cpp


HEADERS += \
    $$PWD/scanner.h \
    $$PWD/scannerapi.h \
    $$PWD/scannerapp.h \
    $$PWD/rawtobmp.h \
    $$PWD/imagetrans.h \
    $$PWD/imageinfo.h \
    $$PWD/ntdcmsapi.h \
    $$PWD/trans_jpg.h
