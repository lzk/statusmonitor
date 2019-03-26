#include "watcher.h"
#include "statusthread.h"
#include <QMutexLocker>

bool use_status_thread = true;
Watcher::Watcher(QObject *parent)
    : QThread(parent)
    ,statusThread(NULL)
    ,abort(false)
//    ,device_manager(new DeviceManager)
{
    if(use_status_thread){
        statusmanager.clearFile();
        statusThread = new StatusThread(this);
        statusThread->start();
    }
    //init printer list
    getPrinters();
    current_printers = printers;
    current_printers_detail = printers_detail;
}

Watcher::~Watcher()
{
    if(statusThread){
        qobject_cast<StatusThread*>(statusThread)->set_abort();
        delete statusThread;
    }
//    delete device_manager;
    abort = true;
    while(abort)usleep(1000);
}

void Watcher::run()
{
    forever{
        if (abort){
            break;
        }
        watcher_job();
        usleep(1000*1000);
    }
    abort = false;
}

void Watcher::set_current_printer(const QString& printer)
{
    if(!current_printer.compare(printer))
        return;
#if 0
    if(use_status_thread){
        if(current_printer.isEmpty()){
            statusThread->set_current_printer(printer);
        }else{
//            QObject* obj = NULL;
            if(statusThread){
                qobject_cast<StatusThread*>(statusThread)->set_abort();
//                obj = statusThread;
                statusThread->deleteLater();
            }
            statusThread = new StatusThread(this);
            statusThread->set_current_printer(printer);
            statusThread->start();
//            if(obj)
//                delete obj;
        }
    }
#else
    if(statusThread){
        statusThread->set_current_printer(printer);
    }
#endif
    current_printer = printer;
    memset(&current_printer_status ,-1 ,sizeof(current_printer_status));
}

int Watcher::printerlist_compare(QList<PrinterInfo_struct> & ps1,QList<PrinterInfo_struct> & ps2)
{
    if(ps1.count() != ps2.count())
        return -1;
    int ret = 0;
    const PrinterInfo_struct* pps1,*pps2;
    for(int i = 0 ;i < ps1.count() ;i++){
        pps1 = &ps1.at(i);
        pps2 = &ps2.at(i);
        if(strcmp(pps1->printer.connectTo ,pps2->printer.connectTo)){
            ret = -1;
            break;
        }else if(strcmp(pps1->printer.name ,pps2->printer.name)){
            ret = -1;
            break;
        }else if(strcmp(pps1->printer.deviceUri ,pps2->printer.deviceUri)){
            ret = -1;
            break;
        }else if(strcmp(pps1->printer.makeAndModel ,pps2->printer.makeAndModel)){
            ret = -1;
            break;
        }
    }
    return ret;
}

void Watcher::watcher_job()
{
    //update printer list
    getPrinters();
    mutex.lock();
    if(printerlist_compare(current_printers_detail ,printers_detail)){
//    if(current_printers != printers){
        current_printers = printers;
        current_printers_detail = printers_detail;
//        emit update_printerlist();
    }
    mutex.unlock();

    //update current printer status
    if(statusThread && statusThread->is_locked_get_status()){
        return;
    }
    if(!current_printer.isEmpty()){
        PrinterInfo_struct ps;
        int index = get_printer_info(current_printer ,ps);
        if(index >= 0){
            PrinterStatus_struct status;
            memset((void*)&status ,0 ,sizeof (status));
            StatusMonitor::getPrinterStatus(current_printer.toLatin1().constData() ,&status);
            QMutexLocker loker(&mutex);
            PrinterStatus_struct* p_current_printer_status = &current_printer_status;
            if(     (status.PrinterStatus != p_current_printer_status->PrinterStatus)
                    ||(status.trayPaperTrayStatus != p_current_printer_status->trayPaperTrayStatus)
                    ||(status.TonelStatusLevelC != p_current_printer_status->TonelStatusLevelC)
                    ||(status.TonelStatusLevelM != p_current_printer_status->TonelStatusLevelM)
                    ||(status.TonelStatusLevelY != p_current_printer_status->TonelStatusLevelY)
                    ||(status.TonelStatusLevelK != p_current_printer_status->TonelStatusLevelK)
                    ||(status.ErrorCodeGroup != p_current_printer_status->ErrorCodeGroup)
                    ||(status.ErrorCodeID != p_current_printer_status->ErrorCodeID)
                    ||(status.PaperSize != p_current_printer_status->PaperSize)
                    ||(status.PaperType != p_current_printer_status->PaperType)
                    ||(status.NonDellTonerMode != p_current_printer_status->NonDellTonerMode)
                    ||(status.LowTonerAlert != p_current_printer_status->LowTonerAlert)
                    ||(status.job != p_current_printer_status->job)
                    ){
                *p_current_printer_status = status;
                current_printers_detail[index].status = status;
                update_printer_status(current_printers_detail[index]);
            }
        }

    }
}

//static int callback_getPrinters(void* para ,Printer_struct* ps)
//{
//    Watcher* worker = (Watcher*)para;
//    worker->setPrinters(ps);
//    return worker->isabort() ?0 :1;
//}

    static int callback_getPrinters(void* para,PrinterInfo_struct* ps)
    {
        Watcher* worker = (Watcher*)para;
        worker->setPrinters(ps);

        return worker->isabort() ?0 :1;
    }

    void Watcher::setPrinters(Printer_struct* ps)
    {
        printers << ps->name;
        PrinterInfo_struct pis;
        pis.printer = *ps;
        memset((void*)&pis.status ,0xff ,sizeof(pis.status));
        printers_detail << pis;
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
//        StatusManager().getPrintersFromFile(callback_getPrinters ,(void*)this);
    }

    int Watcher::get_printer_info(const QString& printer_name ,PrinterInfo_struct& printer_info)
    {
        int ret = -1;
        QMutexLocker locker(&mutex);

        PrinterInfo_struct* this_printer_info = NULL;
        for(int i = 0 ;i < current_printers_detail.count() ;i++){
            if(!printer_name.compare(current_printers_detail[i].printer.name)){
                this_printer_info = &current_printers_detail[i];
                ret = i;
                break;
            }
        }
        if(this_printer_info){
            printer_info = *this_printer_info;
        }
        return ret;
    }

    int Watcher::get_printer_list(QList<PrinterInfo_struct>& pslist)
    {
        QMutexLocker locker(&mutex);
        pslist = current_printers_detail;
        return 0;
    }
