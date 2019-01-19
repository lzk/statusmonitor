#include "watcher.h"
#include "statusthread.h"
#include <QMutexLocker>
Watcher::Watcher(QObject *parent)
    : QThread(parent)
{
    statusThread = new StatusThread();
    statusThread->start();

    connect(&timer ,SIGNAL(timeout()) ,this ,SLOT(timerOut()));
    timer.start(100);
}

Watcher::~Watcher()
{
    delete statusThread;
}

void Watcher::run()
{
    exec();
}

void Watcher::set_current_printer(const QString& printer)
{
    current_printer = printer;
}

void Watcher::timerOut()
{
    //update printer list
    getPrinters();

    //update printer status
    PrinterStatus_struct status;
    StatusMonitor::getPrinterStatus(current_printer.toLatin1().constData() ,&status);

    {
        QMutexLocker loker(&mutex);
//        if(current_printers_detail != printers_detail){
            current_printers = printers;
            current_printers_detail = printers_detail;
//            update_printerlist(printers_detail);
//        }
        if(     (status.PrinterStatus != current_printer_status.PrinterStatus)
                ||(status.trayPaperTrayStatus != current_printer_status.trayPaperTrayStatus)
                ||(status.TonelStatusLevelC != current_printer_status.TonelStatusLevelC)
                ||(status.TonelStatusLevelM != current_printer_status.TonelStatusLevelM)
                ||(status.TonelStatusLevelY != current_printer_status.TonelStatusLevelY)
                ||(status.TonelStatusLevelK != current_printer_status.TonelStatusLevelK)
                ||(status.ErrorCodeGroup != current_printer_status.ErrorCodeGroup)
                ||(status.ErrorCodeID != current_printer_status.ErrorCodeID)
                ||(status.PaperSize != current_printer_status.PaperSize)
                ||(status.PaperType != current_printer_status.PaperType)
                ||(status.NonDellTonerMode != current_printer_status.NonDellTonerMode)
                ||(status.LowTonerAlert != current_printer_status.LowTonerAlert)
                ){
            current_printer_status = status;
            update_printer_status(status);
        }
    }

}

static int callback_getPrinters(void* para,PrinterInfo_struct* ps)
{
    Watcher* worker = (Watcher*)para;
    worker->setPrinters(ps);
    return 1;
}

void Watcher::setPrinters(PrinterInfo_struct* ps)
{
    printers << ps->printer.name;
    printers_detail << *ps;
}

void Watcher::getPrinters()
{
    printers.clear();
    printers_detail.clear();
    StatusMonitor::getPrinters(callback_getPrinters ,(void*)this);
}

int Watcher::get_printer_info(const QString& printer_name ,PrinterInfo_struct& printer_info)
{
    int ret = -1;
    QMutexLocker locker(&mutex);

    PrinterInfo_struct* this_printer_info = NULL;
    for(int i = 0 ;i < current_printers_detail.count() ;i++){
        if(!printer_name.compare(current_printers_detail[i].printer.name)){
            this_printer_info = &current_printers_detail[i];
            break;
        }
    }
    if(this_printer_info){
        printer_info = *this_printer_info;
        ret = 0;
    }
    return ret;
}
