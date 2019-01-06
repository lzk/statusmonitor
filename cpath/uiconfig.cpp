#include "uiconfig.h"

static bool _isDeviceSupported(Printer_struct* ps)
{
//    LOGLOG("test found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
//    if(UIConfig::ModelSerial_unknown == UIConfig::getModelSerial(ps))
//        return false;
    return true;
}

UIConfig::UIConfig(QObject *parent) :
    QObject(parent)
{
}

void UIConfig::initConfig()
{
    //config status server thread
//    filepath = "/tmp/.hornet";
    statusKey = "test/status/";
    printersKey = "test/printerlist/";
//    lockfile = "/tmp/.hornet";

    //config tomcat supported printer model
    isDeviceSupported = _isDeviceSupported;
}

UIConfig::ModelSerial UIConfig::getModelSerial(Printer_struct* ps)
{
    ModelSerial ms = ModelSerial_unknown;
    QString makeAndModel(ps->makeAndModel);
    if(makeAndModel.startsWith("Lenovo L100D")){
        ms = ModelSerial_L;
    }else if(makeAndModel.startsWith("Lenovo L100WW")){
        ms = ModelSerial_L;
    }else if(makeAndModel.startsWith("Lenovo L100DW")){
        ms = ModelSerial_L;
    }else if(makeAndModel.startsWith("Lenovo L100")){
        ms = ModelSerial_L;
    }else if(makeAndModel.startsWith("Lenovo M100W")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M100D")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M100")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M101DW")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M101W")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M101")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M102W")){
        ms = ModelSerial_M;
    }else if(makeAndModel.startsWith("Lenovo M102")){
        ms = ModelSerial_M;
    }
    return ms;
}
