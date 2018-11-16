#include "uiconfig.h"

static bool _isDeviceSupported(Printer_struct* ps)
{
    LOGLOG("test found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
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
