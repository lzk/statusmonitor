#include "worker.h"
#include <QVariant>
#include "uiconfig.h"

#include <QDebug>
#include <QString>
#include "lshell.h"
Worker::Worker(QObject *parent) :
    QObject(parent)
  ,cmd_status(0)
  ,lshell(new LShell(&device))
{
}

Worker::~Worker()
{
}

void Worker::cmdFromUi(int cmd ,const QString& printer_name ,QVariant data)
{
    Q_UNUSED(data);
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
            result = m_statusMonitor.getPrinterStatus(printer->name ,status);
            value.setValue(printerInfo);
        }
        cmdResult(cmd ,result ,value);
    }
        break;
    case UIConfig::LS_CMD_GetCopy:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            copycmdset device_data;
            device->open();
            lshell->copy_get_para(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_COPY:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            copycmdset device_data = value.value<copycmdset>();
            device->open();
            lshell->copy(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_apply:
    case UIConfig::LS_CMD_WIFI_apply_noread:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_wifi_get device_data = value.value<cmdst_wifi_get>();
            device->open();
            lshell->wifi_apply(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_get:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_wifi_get device_data;
            device->open();
            lshell->wifi_get_para(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_getAplist:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_aplist_get device_data;
            device->open();
            lshell->wifi_get_aplist(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PASSWD_set:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_passwd device_data = value.value<cmdst_passwd>();
            device->open();
            lshell->password_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PASSWD_get:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_passwd device_data;
            device->open();
            lshell->password_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PASSWD_confirm:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_passwd device_data = value.value<cmdst_passwd>();
            device->open();
            lshell->password_confirm(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_GetWifiStatus:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_wifi_status device_data;
            device->open();
            lshell->wifi_get_status(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_TonerEnd_Get:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_tonerEnd device_data;
            device->open();
            lshell->tonerend_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_TonerEnd_Set:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_tonerEnd device_data = value.value<cmdst_tonerEnd>();
            device->open();
            lshell->tonerend_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PSaveTime_Get:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_PSave_time device_data;
            device->open();
            lshell->savetime_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PSaveTime_Set:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_PSave_time device_data = value.value<cmdst_PSave_time>();
            device->open();
            lshell->savetime_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PowerOff_Get:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_powerOff_time device_data;
            device->open();
            lshell->poweroff_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_PowerOff_Set:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_powerOff_time device_data = value.value<cmdst_powerOff_time>();
            device->open();
            lshell->poweroff_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_GetV4:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            net_info_st device_data;
            device->open();
            lshell->ipv4_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_SetV4:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            net_info_st device_data = value.value<net_info_st>();
            device->open();
            lshell->ipv4_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_GetV6:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            net_ipv6_st device_data;
            device->open();
            lshell->ipv6_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_NET_SetV6:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            net_ipv6_st device_data = value.value<net_ipv6_st>();
            device->open();
            lshell->ipv6_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_Set_UserConfig:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_userconfig device_data = value.value<cmdst_userconfig>();
            device->open();
            lshell->userconfig_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_Get_UserConfig:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_userconfig device_data;
            device->open();
            lshell->userconfig_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_Set_SoftAp:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_softap device_data = value.value<cmdst_softap>();
            device->open();
            lshell->wifi_softap_set(&device_data);
            device->close();
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_WIFI_Get_SoftAp:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_softap device_data;
            device->open();
            lshell->wifi_softap_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    case UIConfig::LS_CMD_PRN_FusingScReset:
        if(printer){
            device = deviceManager.getDevice(printer->deviceUri);
            cmdst_fusingScReset device_data;
            device->open();
            lshell->fusingsc_get(&device_data);
            device->close();
            value.setValue(device_data);
        }
        cmdResult(cmd ,result ,value);
        break;
    default:
        break;
    }
    cmd_status = 0;
}

DeviceIO* Worker::getDevice(const char* device_uri)
{
    device = deviceManager.getDevice(device_uri);
    return device;
}

void callback_getPrinters(void* para,PrinterInfo_struct* ps)
{
    Worker* worker = (Worker*)para;
    strcpy(ps->printer.connectTo ,worker->getDevice(ps->printer.deviceUri)->getDeviceAddress());
    worker->setPrinters(ps);
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
    m_statusMonitor.getPrinters(callback_getPrinters ,(void*)this);
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
