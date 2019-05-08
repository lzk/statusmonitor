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
    watcher = new Watcher(this);
//    connect(watcher ,SIGNAL(update_printer_status()) ,this ,SLOT(update_printer_status(PrinterInfo_struct)));
    connect(watcher ,SIGNAL(update_current_printer_status()) ,this ,SLOT(update_current_printer_status()));
    connect(watcher ,SIGNAL(update_printerlist()) ,this ,SLOT(update_printerlist()));
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
//    Printer_struct ps;
//    Printer_struct* printer;
//    int index = watcher->get_printer_from_current_list(printer_name ,ps);
//    if(index < 0){
//        printer = NULL;
//    }else{
//        printer = &ps;
//    }
    int result = -1;

    switch (cmd) {
    case UIConfig::CMD_SetCurrentPrinter:
        watcher->set_current_printer(printer_name);
        value = 1;
        result = 0;
        cmdResult(cmd ,result ,value);
        break;

    case UIConfig::CMD_GetPrinters:
        update_printerlist();
        break;

    case UIConfig::CMD_GetStatus:
        update_current_printer_status();
        break;

//    case UIConfig::CMD_GetStatus:
//        if(printer){
//            PrinterInfo_struct printerInfo;
//            strcpy(printerInfo.printer.name ,printer->name);
//            PrinterStatus_struct* status = &printerInfo.status;
//            result = StatusMonitor::getPrinterStatus(printer->name ,status);
//            value.setValue(printerInfo);

//        }
//        cmdResult(cmd ,result ,value);
//        break;

    case UIConfig::CMD_GetJobs:
    {
//        if(printer){
            Jobs_struct jobs;
            jobs.current_page = data.toInt();
            Tomcat::get_job_history(&jobs);
            value.setValue(jobs);
            cmdResult(cmd ,0 ,value);
//        }
    }
        break;
    default:
        break;
    }
    cmd_status = 0;
}

extern int usb_error_printing;
extern int usb_error_busy;
void Worker::update_current_printer_status()
{
    PrinterInfo_struct ps;
    watcher->get_currentprinter_info(ps);
    if(ps.printer.status == usb_error_printing)
        ps.status.PrinterStatus =  0x01;//PS_PRINTING;
    else if(ps.printer.status == usb_error_busy)
        ps.status.PrinterStatus = 0x08;//PS_BUSY;
    QVariant value;
    value.setValue<PrinterInfo_struct>(ps);
    cmdResult(UIConfig::CMD_GetStatus ,0 ,value);
}
//void Worker::update_printer_status(const QString& printer_name)
//{
//    PrinterInfo_struct ps;
//    watcher->get_printer_info(printer_name ,ps);
//    QVariant value;
//    value.setValue<PrinterInfo_struct>(ps);
//    cmdResult(UIConfig::CMD_GetStatus ,0 ,value);
//}

void Worker::update_printerlist()
{
    QList<Printer_struct> printer_list;
    QVariant value;
    watcher->get_printer_list(printer_list);
    value.setValue(printer_list);
    cmdResult(UIConfig::CMD_GetPrinters ,0 ,value);
}
