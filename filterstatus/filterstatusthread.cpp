#include "filterstatusthread.h"
#include <cups/sidechannel.h>

FilterStatusThread::FilterStatusThread(QObject *parent)
    : QThread(parent)
{
    abort = false;
//    statusmanager.clearFile();
}

FilterStatusThread::~FilterStatusThread()
{
    abort = true;
    wait();
}

void FilterStatusThread::run()
{
//    PRINTER_STATUS printer_status;
//    int result;

    char data[2049];
    int datalen;
    cups_sc_status_t status;

    QString str;
    forever {
        if (abort){
            usleep(100 * 1000);
            return;
        }

        /* Tell cupsSideChannelDoRequest() how big our buffer is, less 1 byte for
           nul-termination... */
        datalen = sizeof(data) - 1;

        /* Get the IEEE-1284 device ID, waiting for up to 1 second */
        status = cupsSideChannelDoRequest(CUPS_SC_CMD_GET_DEVICE_ID, data, &datalen, 1.0);

        /* Use the returned value if OK was returned and the length is non-zero */
        if (status == CUPS_SC_STATUS_OK && datalen > 0)
          data[datalen] = '\0';
        else
          data[0] = '\0';

        str = QString("echo %1 >> /tmp/abcd.log").arg(data);
        system(str.toLatin1().constData());


#if 0
        printers.clear();
        printerlist.clear();
//        statusmanager.clearPrintersOfFile();
        cupsmanager.getPrinters(callback_getPrinters ,this);
        statusmanager.savePrintersToFile(printers);

        foreach (Printer_struct printer, printers) {
            if (abort)
                return;
            mutex.lock();
            if(current_printer.compare(printer.name)){
                mutex.unlock();
                continue;
            }
            mutex.unlock();
            result = getStatusFromDevice(devicemanager ,&printer ,&printer_status);
            if(result){
                LOGLOG("get status from device %s:fail!result %d" ,printer.name ,result);
                QMutexLocker locker(&mutex);
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
                QMutexLocker locker(&mutex);
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
        }
#endif
        sleep(6);
    }
}
