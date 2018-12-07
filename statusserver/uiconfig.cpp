#include "uiconfig.h"

static bool _isDeviceSupported(Printer_struct* ps)
{
    LOGLOG("tomcat found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
    return true;
}

UIConfig::UIConfig(QObject *parent) :
    QObject(parent)
{
}

void UIConfig::initConfig()
{
    //config status server thread
//    filepath = "/tmp/.tomcat";
    statusKey = "tomcat/status/";
    printersKey = "tomcat/printerlist/";
//    lockfile = "/tmp/.locktomcat";

    //config tomcat supported printer model
    isDeviceSupported = _isDeviceSupported;
}
