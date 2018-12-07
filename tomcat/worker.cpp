#include "worker.h"
#include <QVariant>
#include "uiconfig.h"

#include <QDebug>
#include <QString>

Worker::Worker(QObject *parent) :
    QObject(parent)
  ,cmd_status(0)
{
}

Worker::~Worker()
{
}

void Worker::cmdFromUi(int cmd ,QVariant data)
{
    Q_UNUSED(data);
    if(cmd_status)
        return;
    cmd_status = 1;

    QVariant value;

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

#ifdef TOMCAT
    case UIConfig::CMD_GetJobs:{
        getJobs();
        value.setValue(jobs);
        cmdResult(cmd ,0 ,value);
    }
        break;
#endif

    case UIConfig::CMD_GetStatus:{
        QString printer_name = data.toString();
        Printer_struct* printer = NULL;
        foreach (PrinterInfo_struct pi, printers_detail) {
            if(!printer_name.compare(pi.printer.name)){
                printer = &pi.printer;
                break;
            }
        }
        int result = -1;
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

int callback_getPrinters(void* para,PrinterInfo_struct* ps)
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
    m_statusMonitor.getPrinters(callback_getPrinters ,(void*)this);
}

#ifdef TOMCAT
void Worker::setJobs(const char* str)
{
    LOGLOG("job:%s" ,str);
    jobs << QString(str);
}

void callback_getJobs(void*para ,const char* str)
{
    Worker* worker = (Worker*)para;
    worker->setJobs(str);
}

void Worker::getJobs()
{
    jobs.clear();
    m_tomcat.getJobHistory(callback_getJobs ,this);
}
#endif
