INCLUDEPATH += $${PWD}
LIBS += -L$${OUT_PWD}/../statusmonitor -lstatusmonitor

unix:!macx: LIBS += -L$${PWD} -lusb-1.0  -lcups

macx: LIBS += -L/Volumes/work/software/libusb -lusb-1.0 -lcups

