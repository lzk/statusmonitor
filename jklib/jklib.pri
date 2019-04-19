
equals(TEMPLATE ,"app"){
LIBS += $${OUT_PWD}/../jklib/libjklib.a
}
include("../jkinterface/jkinterface.pri")
include("../statusmonitor/statusmonitor.pri")
include(../common/common.pri)
include(../lshell/lshell.pri)
include(../scanner/scanner.pri)
