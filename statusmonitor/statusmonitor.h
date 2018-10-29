#ifndef STATUSMONITOR_H
#define STATUSMONITOR_H

#include "api_libcups.h"
#include "fingermanager.h"
#include "log.h"
#include "error.h"

enum{
    DCLASS_unknown,
    DCLASS_usb,
    DCLASS_network
};

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
typedef void (*CALLBACK_getPrinterInfo)(void* ,PrinterInfo_struct*);
class StatusMonitor
{
    
public:
    StatusMonitor();

    int getPrinters(CALLBACK_getPrinterInfo ,void*);
    int getJobs(CALLBACK_getJob ,void*,const char* printer = 0 ,const char* which = 0 ,const char* users = 0);
    static int getPrinterStatus(const char* name ,PrinterStatus_struct* ps);
    static int getPrinterClass(const char* device_uri);
    const char* getDefaultPrinter();
    static int resolve_uri(const char* device_uri ,char* buffer ,int bufsize);

    int getJobHistory(CALLBACK_getJobHistory callback,void* para);

    static bool AnyTonerReachLevel1(const PrinterStatus_struct& m_PrinterStatus);
    static bool IsNonDellTonerMode(const PrinterStatus_struct& m_PrinterStatus);
    static bool OnlyColorTonerEmpty(const PrinterStatus_struct& m_PrinterStatus);
};

#endif // STATUSMONITOR_H
