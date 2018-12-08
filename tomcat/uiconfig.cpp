#include "uiconfig.h"

#include "toecconfig.h"
static bool _isDeviceSupported(Printer_struct* ps)
{
    LOGLOG("tomcat found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
//    if(!QString(ps->makeAndModel).startsWith("Lenovo M7208W"))
    if(!QString(ps->makeAndModel).startsWith("OEP3300CDN"))
        return false;
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

    ui_server_path = SERVER_PATH;

    //config tomcat supported printer model
    isDeviceSupported = _isDeviceSupported;
}
