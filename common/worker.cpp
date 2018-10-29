#include "worker.h"
#include <QVariant>
#include "DataStruct.h"

#include <QDebug>
#include <QString>

Worker::Worker(QObject *parent) :
    QObject(parent)
  ,cmd_status(0)
{
    thread_server.start();
    connect(&thread_server ,SIGNAL(client_cmd(QString)) ,this ,SIGNAL(client_cmd(QString)) ,Qt::DirectConnection);
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
    case DataStruct::CMD_GetDefaultPrinter:{
        QString default_printer(m_statusMonitor.getDefaultPrinter());
        value.setValue(default_printer);
        cmdResult(cmd ,0 ,value);
    }
        break;
    case DataStruct::CMD_GetPrinters:
        getPrinters();
        value.setValue(printers_detail);
        cmdResult(cmd ,0 ,value);
        break;

    case DataStruct::CMD_GetJobs:{
        getJobs();
        value.setValue(jobs);
        cmdResult(cmd ,0 ,value);
    }
        break;

    case DataStruct::CMD_GetStatus:{
        QString printer_name = data.toString();
        PrinterInfo_struct printerInfo;
        strcpy(printerInfo.printer.name ,printer_name.toLocal8Bit().constData());
//        sprintf(printerInfo.printer.name ,"%s",printer_name.toLocal8Bit().constData());
        PrinterStatus_struct* status = &printerInfo.status;
        int result = m_statusMonitor.getPrinterStatus(printer_name.toLocal8Bit().constData() ,status);
        value.setValue(printerInfo);
        cmdResult(cmd ,result ,value);
    }
        break;
    default:
        break;
    }
    cmd_status = 0;
}

void callback_getPrinters(void* para,PrinterInfo_struct* ps)
{
    Worker* worker = (Worker*)para;
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
    m_statusMonitor.getJobHistory(callback_getJobs ,this);
}
