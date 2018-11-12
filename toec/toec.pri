INCLUDEPATH += $${PWD}/../toec \
               $${PWD}/../jkinterface

LIBS += -L$${OUT_PWD}/../toec -ltoec

macx: LIBS += -L/Volumes/work/software/libusb

HEADERS += \
    $${PWD}/tomcat.h \
    $${PWD}/filter_check_finger.h \
