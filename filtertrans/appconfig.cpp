#include "appconfig.h"
#include "appserver.h"
#include "commonapi.h"
#include <unistd.h>

//const QString app_name = QString::fromUtf8("打印机状态监视器");
FileLocker app_file_locker;
AppServer* app_server;

extern
int (* getpidvid)(const QString& modelname ,int* pid ,int* vid);
//log file var
extern const char* log_app_name;
extern const char* app_version;
//usb error control var
extern int usb_error_printing;
static bool _isDeviceSupported(Printer_struct* ps)
{
//    LOGLOG("tomcat found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
//    if(!QString(ps->makeAndModel).startsWith("Lenovo M101DW"))
//    if(!QString(ps->makeAndModel).startsWith("Lenovo M7208W"))
    if(!QString(ps->makeAndModel).startsWith("OEP3300CDN"))
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

AppConfig::AppConfig(QObject *parent) :
    QObject(parent)
{
}

int AppConfig::initConfig()
{
    log_app_name = "filtertrans";
    app_version = "1.0.2";
    log_init();
    LOGLOG("--------%s v%s-------" ,log_app_name ,app_version);
    const char* app_locker_file = "/tmp/.tjgd1zsm_locker";
    if(app_file_locker.trylock(app_locker_file)){
        LOGLOG("app had been locked!");
        return -1;
    }
    if(is_app_running(SERVER_PATH)){
        LOGLOG("socket working!");
        return -2;
    }

//#ifndef DEBUG_TO_STDERR
//    //release as deaemon
//    int result = daemon(0 ,0);
//    if(!result){
//        LOGLOG("daemon success!");
//    }
//#endif
    //config status server thread
//    status_file = "/tmp/.toecstatus";
//    status_lock_file = "/tmp/.locktoecstatus";
//    statusKey = "statusmonitor/status/";
//    printersKey = "statusmonitor/printerlist/";

//    ui_server_path = SERVER_PATH;
    usb_error_printing = 0x01;

    //config tomcat supported printer model
    isDeviceSupported = _isDeviceSupported;
    getpidvid = _getpidvid;

    app_server = new AppServer(SERVER_PATH);
    return 0;
}

#include <QFile>
void AppConfig::exit_app()
{
//    QFile::remove(status_file);
//    QFile::remove(status_lock_file);
    delete app_server;
    app_file_locker.unlock();
}
