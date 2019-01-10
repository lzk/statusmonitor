INCLUDEPATH += $${PWD}

macx{
    LIBS += -L$${PWD}/../libs/mac -lntdcms
}else{
    unix{
        contains(QT_ARCH, i386) {
            LIBS += -L$${PWD}/../libs/linux32 -lntdcms
        }else{
            LIBS += -L$${PWD}/../libs/linux64 -lntdcms
        }
    }
}

SOURCES += \
    $$PWD/scanner.cpp \
    $$PWD/scannerapi.cpp \
    $$PWD/scannerapp.cpp \
    $$PWD/rawtobmp.cpp \
    $$PWD/imagetrans.cpp \
    $$PWD/ntdcmsapi.cpp


HEADERS += \
    $$PWD/scanner.h \
    $$PWD/scannerapi.h \
    $$PWD/scannerapp.h \
    $$PWD/rawtobmp.h \
    $$PWD/imagetrans.h \
    $$PWD/imageinfo.h \
    $$PWD/ntdcmsapi.h
