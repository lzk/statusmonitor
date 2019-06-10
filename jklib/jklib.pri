
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

#CONFIG(debug ,debug|release){
LIBS += \
     -L $${OUT_PWD}/../jklib -ljklib \

#}else{
#LIBS += \
#    $${OUT_PWD}/../jklib/libjklib.a \

#}

LIBS += \
    -L$${PWD}/../libs \
    -lusb-1.0 \
    -lcups \
    -llnthr8zcl \

!contains(CONFIG ,static){
LIBS += \
    -ljpeg
}

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
#                    -L$${PWD}/../libs/linux64 \
#                    $${PWD}/../libs/linux64/libnetsnmp.a \
                    -lnetsnmp

        }
    }
}
