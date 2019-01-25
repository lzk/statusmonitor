#include "worker.h"
#include <QVariant>
#include "uiconfig.h"

#include <QDebug>
#include <QString>
#include <QDir>
#include <QDateTime>
#include "lshell.h"
Worker::Worker(QObject *parent) :
    QObject(parent)
  ,cmd_status(0)
  ,deviceManager(new DeviceManager)
  ,lshell(new LShell(deviceManager))
  ,scanner(new ScannerApp(deviceManager))
{
}

Worker::~Worker()
{
    delete deviceManager;
    delete lshell;
    delete scanner;
}


static void scan_callback(void* para);
void Worker::cmdFromUi(int cmd ,const QString& printer_name ,QVariant data)
{
    if(cmd_status)
        return;
    cmd_status = 1;

    QVariant value;
    Printer_struct* printer = get_printer(printer_name);
    int result = -1;

    switch (cmd) {
    case UIConfig::CMD_GetDefaultPrinter:{
        QString default_printer;
        if(!printers.isEmpty())
            default_printer= printers.first();
        value.setValue(default_printer);
        cmdResult(cmd ,0 ,value);
    }
        break;
    case UIConfig::CMD_GetPrinters:
        getPrinters();
        value.setValue(printers_detail);
        cmdResult(cmd ,0 ,value);
        break;

    case UIConfig::CMD_GetStatus:{
        if(printer){
            PrinterInfo_struct printerInfo;
            strcpy(printerInfo.printer.name ,printer->name);
            PrinterStatus_struct* status = &printerInfo.status;
            result = StatusMonitor::getDeviceStatus(deviceManager ,printer ,status);
            value.setValue(printerInfo);
        }
        cmdResult(cmd ,result ,value);
    }
        break;
    case UIConfig::CMD_Scan:
        if(printer){
            ScanSettings device_data = data.value<ScanSettings>();
            const char* imagePath = "/tmp/vop_scan";
            QDir *path = new QDir;
            if(!(path->exists(imagePath)))
            {
                path->mkdir(imagePath);
            }
            delete path;
            QDateTime time = QDateTime::currentDateTime();
            QString str_time = time.toString("yyyy-MM-dd_hh-mm-ss-zzz");
            QByteArray t_ba = str_time.toLatin1();
            sprintf(device_data.filename, "%s/%s.bmp", imagePath, t_ba.constData());

            //only net scan use jpg
//            if(device->type() == DeviceIO::Type_usb)
                device_data.settings.scan_type = Hight_Quality;

            device_data.callback_para = this;
            device_data.callback = scan_callback;
            result = scanner->scan(printer ,&device_data);
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;

    case UIConfig::LS_CMD_GetCopy:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                copycmdset device_data;
                result = lshell->copy_get_para(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_COPY:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                copycmdset device_data = data.value<copycmdset>();
                result = lshell->copy(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_apply:
    case UIConfig::LS_CMD_WIFI_apply_noread:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_wifi_get device_data = data.value<cmdst_wifi_get>();
                result = lshell->wifi_apply(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_get:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_wifi_get device_data;
                result = lshell->wifi_get_para(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;

    case UIConfig::CMD_WIFI_refresh_plus:
        if(printer){
            struct_wifi_refresh_info wifi_refresh_data;
            result = lshell->open(printer);
            if(!result){
//            if(!cmd_status_validate_setting(err)){
//                break;
//            }
                result = lshell->wifi_get_para(&wifi_refresh_data.wifi_para);
                if(!result){
                    cmdst_wifi_status status_data;
                    result = lshell->wifi_get_status(&status_data);
                }
                if(!result){
                    result = lshell->wifi_get_aplist(&wifi_refresh_data.wifi_aplist);
                }
                value.setValue(wifi_refresh_data);
                lshell->close();
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_getAplist:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_aplist_get device_data;
                result = lshell->wifi_get_aplist(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PASSWD_set:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_passwd device_data = data.value<cmdst_passwd>();
                result = lshell->password_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PASSWD_get:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_passwd device_data;
                result = lshell->password_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PASSWD_confirm:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_passwd device_data = data.value<cmdst_passwd>();
                result = lshell->password_confirm(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_GetWifiStatus:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_wifi_status device_data;
                result = lshell->wifi_get_status(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_TonerEnd_Get:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_tonerEnd device_data;
                result = lshell->tonerend_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_TonerEnd_Set:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_tonerEnd device_data = data.value<cmdst_tonerEnd>();
                result = lshell->tonerend_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PSaveTime_Get:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_PSave_time device_data;
                result = lshell->savetime_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PSaveTime_Set:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_PSave_time device_data = data.value<cmdst_PSave_time>();
                result = lshell->savetime_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PowerOff_Get:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_powerOff_time device_data;
                result = lshell->poweroff_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PowerOff_Set:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_powerOff_time device_data = data.value<cmdst_powerOff_time>();
                result = lshell->poweroff_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_GetV4:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                net_info_st device_data;
                result = lshell->ipv4_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_SetV4:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                net_info_st device_data = data.value<net_info_st>();
                result = lshell->ipv4_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_GetV6:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                net_ipv6_st device_data;
                result = lshell->ipv6_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_SetV6:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                net_ipv6_st device_data = data.value<net_ipv6_st>();
                result = lshell->ipv6_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_Set_UserConfig:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_userconfig device_data = data.value<cmdst_userconfig>();
                result = lshell->userconfig_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_Get_UserConfig:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_userconfig device_data;
                result = lshell->userconfig_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_Set_SoftAp:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_softap device_data = data.value<cmdst_softap>();
                result = lshell->wifi_softap_set(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_Get_SoftAp:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_softap device_data;
                result = lshell->wifi_softap_get(&device_data);
                lshell->close();
                value.setValue(device_data);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_FusingScReset:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                result = lshell->fusingsc_reset();
                lshell->close();
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_TonerReset:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                result = lshell->toner_reset();
                lshell->close();
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_DrumReset:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                result = lshell->drum_reset();
                lshell->close();
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_Get_UserCenterInfo:
        if(printer){
            result = lshell->open(printer);
            if(!result){
                cmdst_user_center device_data;
                result = lshell->usercenterinfo_get(&device_data);
                struct_user_center user_center;
                user_center.strPrinterModel = printer->name;
                qDebug()<<printer;
                user_center.user_center = device_data;
                lshell->close();
                value.setValue(user_center);
            }
        }
        cmdResult(cmd ,result ,value);
        break;
    default:
        break;
    }
    cmd_status = 0;
}

int callback_getPrinters(void* para,PrinterInfo_struct* ps)
{
    Worker* worker = (Worker*)para;
//    strcpy(ps->printer.connectTo ,worker->getDevice(&ps->printer)->getDeviceAddress(&ps->printer));
    worker->setPrinters(ps);
    return 1;
}

void Worker::setPrinters(PrinterInfo_struct* ps)
{
    printers << ps->printer.name;
    printers_detail << *ps;
}

void Worker::getPrinters()
{
    printers.clear();
    printers_detail.clear();
    StatusMonitor::getPrinters(callback_getPrinters ,(void*)this);
}

Printer_struct* Worker::get_printer(const QString& printer_name)
{
    Printer_struct* printer = NULL;
    for(int i = 0 ;i < printers_detail.count() ;i++){
        if(!printer_name.compare(printers_detail[i].printer.name)){
            printer = &printers_detail[i].printer;
            break;
        }
    }
    return printer;
}

void Worker::cancel()
{
    scanner->set_cancel(true);
}

void Worker::update_scan_progress(int progress)
{
    emit signal_update_scan_progress(progress);
}

static void scan_callback(void* para)
{
    ScanSettings* settings = (ScanSettings*)para;
    Worker* worker = (Worker*) settings->callback_para;
    worker->update_scan_progress(settings->progress);
//    LOGLOG("scan progress:%d" ,settings->progress);
}
