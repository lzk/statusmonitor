INCLUDEPATH += $${PWD}/../toec \
               $${PWD}/../jkinterface

#LIBS += -L$${OUT_PWD}/../toec -ltjgd1z

#macx: LIBS += -L/Volumes/work/software/libusb

HEADERS += \
    $${PWD}/tomcat.h \
    $${PWD}/toecconfig.h \
    $${PWD}/filter_check_finger.h \

SOURCES += \
    $${PWD}/tomcat.cpp \
    $$PWD/finger.cpp
