#include "uiconfig.h"

#include "toecconfig.h"
const QString app_name = QString::fromUtf8("打印机状态监视器");
static bool _isDeviceSupported(Printer_struct* ps)
{
//    LOGLOG("tomcat found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
//    if(!QString(ps->makeAndModel).startsWith("Lenovo M101DW"))
//    if(!QString(ps->makeAndModel).startsWith("Lenovo M7208W"))
    if(!QString(ps->makeAndModel).startsWith("1OEP3300CDN"))
        return false;
    return true;
}

static int _getpidvid(const QString& makeAndModel ,int* pid ,int* vid)
{
    if(!pid || !vid)
        return -1;
    *vid = 0x0efd;
    *pid = -1;
    if(makeAndModel.startsWith("toec/OEP3300CDN")){
        *pid = 0x002c;
    }
    return (*pid == -1) ?-1 :0;
}
extern
int (* getpidvid)(const QString& modelname ,int* pid ,int* vid);

UIConfig::UIConfig(QObject *parent) :
    QObject(parent)
{
}

extern const char* log_app_name;
extern const char* app_version;
void UIConfig::initConfig()
{
    //config status server thread
    status_file = "/tmp/.toecstatus";
    statusKey = "statusmonitor/status/";
    printersKey = "statusmonitor/printerlist/";
    status_lock_file = "/tmp/.locktoecstatus";

//    ui_server_path = SERVER_PATH;

    //config tomcat supported printer model
    isDeviceSupported = _isDeviceSupported;
    getpidvid = _getpidvid;

    log_app_name = "tomcat";
    app_version = "1.0.3beta";
    log_init();
}

#include <QFile>
void UIConfig::exit_app()
{
    QFile::remove(status_file);
    QFile::remove(status_lock_file);
}
