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
    PRINTER_STATUS status;
    int result;
    forever {
        if (abort)
            return;
        printers.clear();
        printerlist.clear();
//        statusmanager.clearPrintersOfFile();
        cupsmanager.getPrinters(callback_getPrinters ,this);
        statusmanager.savePrintersToFile(printers);

        foreach (Printer_struct printer, printers) {
            if (abort)
                return;
            if(current_printer.compare(printer.name))
                continue;
            result = getStatusFromDevice(devicemanager ,&printer ,&status);
            if(result){
                LOGLOG("get status from device %s:fail!" ,printer.name);
                memset(&status ,0 ,sizeof(status));
//                status.PrinterStatus = PS_ERROR_POWER_OFF;
                if(result == usb_error_printing)
                    status.PrinterStatus = usb_error_printing;
                else if(result == usb_error_scanning)
                    status.PrinterStatus = usb_error_scanning;
                else
                    status.PrinterStatus = PS_UNKNOWN;
//                status.TonelStatusLevelC = -1;
//                status.TonelStatusLevelM = -1;
//                status.TonelStatusLevelY = -1;
//                status.TonelStatusLevelK = -1;
            }else{
//                LOGLOG("get status from device %s:success!" ,printer.name);
//                LOGLOG("status:0x%02x" ,status.PrinterStatus);
//                if(IsStatusAbnormal(status.PrinterStatus)){
//                    status.PrinterStatus = PS_OFFLINE;
        //            status.PrinterStatus = PS_PAUSED;
//                }
            }
            statusmanager.saveStatusToFile(printer.name ,&status);
        }
        sleep(6);
    }
}

void StatusThread::set_current_printer(const QString& printer)
{
    QMutexLocker locker(&mutex);
    current_printer = printer;
}
