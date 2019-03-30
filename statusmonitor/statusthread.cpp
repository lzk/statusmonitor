#include "statusthread.h"
#include "commonapi.h"
#include "status.h"
#include "smconfig.h"
#include "statusmonitor.h"

static int callback_getPrinters(void* para,Printer_struct* ps)
{
    StatusThread* st = static_cast<StatusThread*>(para);
    if(isDeviceSupported && isDeviceSupported(ps)){
        st->printers << *ps;
        st->printerlist << ps->name;
    }
    return st->abort ?0 :1;
}

StatusThread::StatusThread(QObject *parent)
    : QThread(parent)
    , devicemanager(new DeviceManager)
{
    abort = false;
    statusmanager.clearFile();
    get_printer_list();
}

StatusThread::~StatusThread()
{
    abort = true;
    while(abort)usleep(1000);
    delete devicemanager;
}

extern int usb_error_printing;
extern int usb_error_scanning;
extern int usb_error_usb_locked;
extern int usb_error_busy;
void StatusThread::run()
{
    PRINTER_STATUS printer_status;
    int result;
    forever {
        if (abort)
            break;
        get_printer_list();

        foreach (Printer_struct printer, printers) {
            if (abort)
                break;
            mutex.lock();
            if(current_printer.compare(printer.name)){
                mutex.unlock();
                continue;
            }
            mutex.unlock();
            result = getStatusFromDevice(devicemanager ,&printer ,&printer_status);
            if(result == usb_error_printing){
                LOGLOG("now is printing,get status via cups");
            }else{
                if (abort)
                    break;
                mutex.lock();
                if(result){
                    status.PrinterStatus = result;
                }else{
                    status = printer_status;
                }
                statusmanager.saveStatusToFile(printer.name ,&status);
                mutex.unlock();
            }
        }
        sleep(6);
    }
    abort = false;
}

void StatusThread::set_current_printer(const QString& printer)
{
    QMutexLocker locker(&mutex);
    current_printer = printer;
    memset(&status ,-1 ,sizeof(status));
}

void StatusThread::get_printer_list()
{
    printers.clear();
    printerlist.clear();
    StatusMonitor::getPrinters(callback_getPrinters ,this);
    for(int i = 0 ;i < printers.count() ;i++){
        devicemanager->get_device_connect_status(&printers[i]);
        if (abort)
            return;
    }
//    statusmanager.savePrintersToFile(printers);
}

void StatusThread::set_device_id(const QString& printer ,const QString& device_id)
{
    {
        QMutexLocker locker(&mutex);
        if(printer.compare(current_printer)){
            return;
        }
    }
    PRINTER_STATUS printer_status;
    if(!DecodeStatusFromDeviceID(device_id.toLatin1().data() ,&printer_status)){
        statusmanager.saveStatusToFile(printer.toLatin1().constData() ,&printer_status);
    }
}
