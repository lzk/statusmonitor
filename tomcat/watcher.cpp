#include "watcher.h"
#include "statusthread.h"
#include <QMutexLocker>
#include "tomcat.h"

bool use_status_thread = true;
Watcher::Watcher(DeviceManager* _device_manager ,QObject *parent)
    : QThread(parent)
    ,abort(false)
    ,device_manager(_device_manager)
{
    if(use_status_thread){
        statusThread = new StatusThread();
        statusThread->start();
    }else{
        statusThread = NULL;
    }
}

Watcher::~Watcher()
{
    if(statusThread)
        delete statusThread;
    abort = true;
    wait();
}

void Watcher::run()
{
    forever{
        if (abort)
            return;
        timerOut();
        usleep(100*1000);
    }
}

void Watcher::set_current_printer(const QString& printer)
{
    current_printer = printer;
}

void Watcher::timerOut()
{
    //update printer list
    getPrinters();
//    mutex.lock();
//    current_printers = printers;
//    current_printers_detail = printers_detail;
//    mutex.unlock();

    for(int i = 0 ;i < printers_detail.count() ;i++){
        if (abort)
            return;
        strcpy(printers_detail[i].printer.connectTo
               ,device_manager->getDevice(&printers_detail[i].printer)->getDeviceAddress(&printers_detail[i].printer));
    }
    mutex.lock();
    if(current_printers != printers){
        current_printers = printers;
        current_printers_detail = printers_detail;
        emit update_printerlist();
    }
    mutex.unlock();

    //update current printer status
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
                    ){
                *p_current_printer_status = status;
                current_printers_detail[index].status = status;
                update_printer_status(current_printers_detail[index]);
            }
        }

    }
    //update job history info
    Tomcat::update_job_history();
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