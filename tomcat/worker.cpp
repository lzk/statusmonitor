#include "worker.h"
#include <QVariant>
#include "uiconfig.h"
#include "tomcat.h"
#include <QDebug>
#include <QString>

Worker::Worker(QObject *parent) :
    QObject(parent)
  ,cmd_status(0)
{
    watcher = new Watcher(&deviceManager);
    connect(this ,SIGNAL(set_current_printer(QString)) ,watcher ,SLOT(set_current_printer(QString)));
    connect(watcher ,SIGNAL(update_printer_status(PrinterInfo_struct)) ,this ,SLOT(update_printer_status(PrinterInfo_struct)));
    watcher->start();
}

Worker::~Worker()
{
    delete watcher;
}

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
//        getPrinters();
        watcher->get_printer_list(printers_detail);
        value.setValue(printers_detail);
        cmdResult(cmd ,0 ,value);
        break;

    case UIConfig::CMD_GetStatus:
        if(printer){
            PrinterInfo_struct printerInfo;
            strcpy(printerInfo.printer.name ,printer->name);
            PrinterStatus_struct* status = &printerInfo.status;
            result = StatusMonitor::getPrinterStatus(printer->name ,status);
            value.setValue(printerInfo);

        }
        cmdResult(cmd ,result ,value);
        break;

    case UIConfig::CMD_GetJobs:
        if(printer){
            Jobs_struct jobs;
            jobs.current_page = data.toInt();
            Tomcat::get_job_history(&jobs);
            value.setValue(jobs);
            cmdResult(cmd ,0 ,value);
        }
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

static int callback_getPrinters(void* para,PrinterInfo_struct* ps)
{
    Worker* worker = (Worker*)para;
    strcpy(ps->printer.connectTo ,worker->getDevice(ps->printer.deviceUri)->getDeviceAddress());
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

void Worker::update_printer_status(PrinterInfo_struct ps)
{
    LOGLOG("watcher update status");
    QVariant value;
    value.setValue<PrinterInfo_struct>(ps);
    cmdResult(UIConfig::CMD_GetStatus ,0 ,value);
}
