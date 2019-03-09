#include "statusthread.h"
#include "commonapi.h"
#include "status.h"
#include "error.h"
#include "smconfig.h"

static int callback_getPrinters(void* para,Printer_struct* ps)
{
    StatusThread* st = static_cast<StatusThread*>(para);
    if(isDeviceSupported && isDeviceSupported(ps)){
        st->printers << *ps;
        st->printerlist << ps->name;
//        st->statusmanager.savePrinterToFile(ps);
    }
    return st->abort ?0 :1;
}

StatusThread::StatusThread(QObject *parent)
    : QThread(parent)
    , devicemanager(new DeviceManager)
{
    abort = false;
    statusmanager.clearFile();
}

StatusThread::~StatusThread()
{
    abort = true;
    wait();
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
        if (abort){
            usleep(100*1000);
            return;
        }
        printers.clear();
        printerlist.clear();
//        statusmanager.clearPrintersOfFile();
        cupsmanager.getPrinters(callback_getPrinters ,this);
        statusmanager.savePrintersToFile(printers);

        foreach (Printer_struct printer, printers) {
            if (abort){
                usleep(100*1000);
                return;
            }
            mutex.lock();
            if(current_printer.compare(printer.name)){
                mutex.unlock();
                continue;
            }
            mutex.unlock();
            result = getStatusFromDevice(devicemanager ,&printer ,&printer_status);
//            for(int i = 0 ;i < 3 ;i ++)
//            {
//                result = getStatusFromDevice(devicemanager ,&printer ,&printer_status);
//                if(!result)
//                    break;
//                if (abort){
//                    usleep(100*1000);
//                    return;
//                }
//            }
            mutex.lock();
            if(result){
                LOGLOG("get status from device %s:fail!result:0x%02x" ,printer.name ,result);
                status.PrinterStatus = result;
//                status.PrinterStatus = PS_ERROR_POWER_OFF;
//                if(result == usb_error_printing)
//                    status.PrinterStatus = usb_error_printing;
//                else if(result == usb_error_scanning)
//                    status.PrinterStatus = usb_error_scanning;
//                else if(result == usb_error_busy)
//                    status.PrinterStatus = usb_error_busy;
//                else
//                    status.PrinterStatus = PS_UNKNOWN;
//                status.TonelStatusLevelC = -1;
//                status.TonelStatusLevelM = -1;
//                status.TonelStatusLevelY = -1;
//                status.TonelStatusLevelK = -1;
            }else{
                status = printer_status;
//                memcpy(&status ,&printer_status ,sizeof(status));
//                LOGLOG("get status from device %s:success!" ,printer.name);
//                LOGLOG("status:0x%02x" ,status.PrinterStatus);
//                if(IsStatusAbnormal(status.PrinterStatus)){
//                    status.PrinterStatus = PS_OFFLINE;
        //            status.PrinterStatus = PS_PAUSED;
//                }
            }
            statusmanager.saveStatusToFile(printer.name ,&status);
            locker_get_status = false;
            mutex.unlock();
        }
        sleep(6);
    }
}

void StatusThread::set_current_printer(const QString& printer)
{
    QMutexLocker locker(&mutex);
    current_printer = printer;
    memset(&status ,0 ,sizeof(status));
    locker_get_status = true;
}

bool StatusThread::is_locked_get_status()
{
    QMutexLocker locker(&mutex);
    return locker_get_status;
}
