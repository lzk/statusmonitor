#include "uiconfig.h"
#include "commonapi.h"
#include "filelocker.h"
#include "appserver.h"
#include <QFile>
#include "toecconfig.h"

const QString app_name = QString::fromUtf8("打印机状态监视器");
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
    }else if(makeAndModel.startsWith("lenovo/M7268W")){
        *vid = 0x17ef;
        *pid = 0x563a;
    }
    return (*pid == -1) ?-1 :0;
}

UIConfig::UIConfig(QObject *parent) :
    QObject(parent)
{
}

int UIConfig::initConfig()
{
    log_app_name = "tjgd1zsmui";
    app_version = "1.0.12beta";
    log_init();
    LOGLOG("--------%s v%s-------" ,log_app_name ,app_version);
    QString str;
    str = get_string_from_shell_cmd("uname -a");
    LOGLOG("%s" ,str.toLatin1().constData());
    str = get_string_from_shell_cmd("cat /etc/issue");
    LOGLOG("%s\n\n" ,str.toLatin1().constData());
//    str = get_string_from_shell_cmd("who" ,1);
//    LOGLOG("%s\n\n" ,str.toLatin1().constData());
//    str = get_string_from_shell_cmd("whoami");
//    LOGLOG("%s\n\n" ,str.toLatin1().constData());
//    str = get_string_from_shell_cmd("echo $DISPLAY");
//    LOGLOG("%s\n\n" ,str.toLatin1().constData());
//    LOGLOG("%s\n\n" ,getenv("DISPLAY"));

    const char* app_locker_file = "/tmp/.tjgd1zsmui_locker";
    if(app_file_locker.trylock(app_locker_file)){
        LOGLOG("app had been locked!");
        return -1;
    }
    if(is_app_running(SERVER_PATH_STM)){
        LOGLOG("socket working!");
        return -2;
    }

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

    app_server = new AppServer(SERVER_PATH_STM);
    return 0;
}

void UIConfig::exit_app()
{
//    QFile::remove(status_file);
//    QFile::remove(status_lock_file);
    app_file_locker.unlock();
    delete app_server;
}
