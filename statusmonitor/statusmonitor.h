#ifndef STATUSMONITOR_H
#define STATUSMONITOR_H

#include "status.h"
#include "jkinterface.h"
#include "error.h"
typedef
struct PrinterStatus_struct
{
    int PrinterStatus;
    int trayPaperTrayStatus;
    int	TonelStatusLevelC;
    int	TonelStatusLevelM;
    int	TonelStatusLevelY;
    int	TonelStatusLevelK;
    int ErrorCodeGroup;
    int ErrorCodeID;
    int PaperSize;
    int PaperType;
    int NonDellTonerMode;
    int LowTonerAlert;
}
    PrinterStatus_struct;

typedef struct PrinterInfo_struct
{
    PrinterStatus_struct status;
    Printer_struct printer;
}
    PrinterInfo_struct;

typedef int (*CALLBACK_getPrinterInfo)(void* ,PrinterInfo_struct*);
class StatusMonitor
{
    
public:
    StatusMonitor();

    static int getPrinters(CALLBACK_getPrinterInfo ,void*);
//    int getPrinters(CALLBACK_getPrinters ,void*);
    static int getPrinterStatus(const char* printer ,PrinterStatus_struct* ps);
    static int getDeviceStatus(DeviceIO* device ,PrinterStatus_struct* ps);

    static bool AnyTonerReachLevel1(const PrinterStatus_struct& m_PrinterStatus);
    static bool IsNonDellTonerMode(const PrinterStatus_struct& m_PrinterStatus);
    static bool OnlyColorTonerEmpty(const PrinterStatus_struct& m_PrinterStatus);

private:
};

#endif // STATUSMONITOR_H
