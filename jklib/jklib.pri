
QT += core network

HEADERS += \
    $$PWD/jklib.h

INCLUDEPATH += \
            $${PWD} \
            $${PWD}/../libs \
            $${PWD}/../libs/cups-2.2.8 \
            $${PWD}/../jkinterface \
            $${PWD}/../statusmonitor \
            $${PWD}/../common \
            $${PWD}/../lshell \
            $${PWD}/../scanner \

mac{

    DEFINES += JK_OS_MAC

LIBS += \
    -L/Volumes/work/software/libusb \
    -lnetsnmp \
    -L$${PWD}/../libs/mac \

}else{
    unix{
        contains(QT_ARCH, i386){
                LIBS += \
                    -L$${PWD}/../libs/linux32 \
                    $${PWD}/../libs/linux32/libnetsnmp.a \

        }else{
                LIBS += \
                    -L$${PWD}/../libs/linux64 \
                    $${PWD}/../libs/linux64/libnetsnmp.a \

        }
    }
}

LIBS += \
    $${OUT_PWD}/../jklib/libjklib.a \
    -L$${PWD}/../libs \
    -lusb-1.0 \
    -lcups \
    -llnthr8zcl \
    -ljpeg

!contains(CONFIG ,static){
    #            LIBS += $${PWD}/../libs/mac/libjpeg.a
    #            LIBS += $${PWD}/../libs/linux32/libjpeg.a
    #            LIBS += $${PWD}/../libs/linux64/libjpeg.a
}
