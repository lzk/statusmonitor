#ifndef FINGERCOMMON_H
#define FINGERCOMMON_H

#define SUPPORT_FINGERMANAGER 0
#include "jkinterface.h"
#include "commonapi.h"
#if SUPPORT_FINGERMANAGER
#include "cupsmanager.h"
#endif
#include "FingCmd.h"

#include <QStringList>
class FingerCommon
{
public:
    FingerCommon();
#if SUPPORT_FINGERMANAGER
    QList<Printer_struct> printers;
    QStringList printerlist;
#endif
    bool IsDeviceConnect(char *pPrinterName);
    int IsDeviceConnectForPrint(char *pPrinterName);
    bool getDevice();
    int IsDeviceFingerOpen(char* pPrinterName);
    int IsDeviceFingerOpenForPrint(char* pPrinterName);
    bool OpenFinger(char* pPrinterName);
    bool CloseFinger(char* pPrinterName);
    int CheckFinger(char* pPrinterName, char* userName);
    bool CancelFingerPrint(char* pPrinterName);
    int DeleteFinger(char* pPrinterName, char* userName);
    int DeleteFingers(char* pPrinterName);

    int AddFinger(char* pPrinterName, int mode, char* userName, int *ret_index);
    int ImportFinger(char* pPrinterName, char* fileName);
    int ExportFinger(char* pPrinterName, char* fileName, int *pdwLen);
    int IsFingerPrint(char* pPrinterName, char* userName, short* pIndex, int mTimeout);




private:
    bool abort;
    DeviceManager devicemanager;
#if SUPPORT_FINGERMANAGER
    CupsManager cupsmanager;
#endif
    FingCmd cmd;
};



#endif // FINGERCOMMON_H
