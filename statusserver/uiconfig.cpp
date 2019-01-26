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
//    status_file = "/tmp/.tomcat";
    statusKey = "tomcat/status/";
    printersKey = "tomcat/printerlist/";
//    status_lock_file = "/tmp/.locktomcat";

    //config tomcat supported printer model
    isDeviceSupported = _isDeviceSupported;
}
