INCLUDEPATH += $${PWD}

HEADERS += \
    ../common/DataStruct.h \
    ../common/worker.h \
    ../common/uinterface.h \
    $$PWD/serverthread.h

SOURCES += \
    ../common/main.cpp \
    ../common/worker.cpp \
    ../common/uinterface.cpp \
    $$PWD/serverthread.cpp

FORMS +=
