#include "statusthread.h"
#include "commonapi.h"
#include "status.h"
#include "error.h"
#include "smconfig.h"

static int callback_getPrinters(void* para,Printer_struct* ps)
{
    StatusThread* st = (StatusThread*)para;
    if(isDeviceSupported && isDeviceSupported(ps)){
        st->printers << *ps;
        st->printerlist << ps->name;
        st->statusmanager.savePrinterToFile(ps);
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

void StatusThread::run()
{
    PRINTER_STATUS status;
    DeviceIO* device;
    int result;
    forever {
        if (abort)
            return;
        printers.clear();
        printerlist.clear();
        statusmanager.clearPrintersOfFile();
        cupsmanager.getPrinters(callback_getPrinters ,this);
        //update printer list
//        statusmanager.clearPrintersOfFile();
//        statusmanager.savePrintersToFile(printerlist);

        foreach (Printer_struct printer, printers) {
            if (abort)
                return;
            result = getStatusFromDevice(devicemanager ,&printer ,&status);
            if(result){
                LOGLOG("get status from device %s:fail!" ,printer.name);
                memset(&status ,0 ,sizeof(status));
//                status.PrinterStatus = PS_ERROR_POWER_OFF;
                status.PrinterStatus = PS_UNKNOWN;
            }else{
//                LOGLOG("get status from device %s:success!" ,printer.name);
//                LOGLOG("status:0x%02x" ,status.PrinterStatus);
                if(IsStatusAbnormal(status.PrinterStatus)){
                    status.PrinterStatus = PS_OFFLINE;
        //            status.PrinterStatus = PS_PAUSED;
                }
            }
            statusmanager.saveStatusToFile(printer.name ,&status);
        }
        sleep(6);
    }
}
