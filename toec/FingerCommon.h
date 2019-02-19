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
    bool IsDeviceConnectForPrint(char *pPrinterName);
    bool getDevice();
    bool IsDeviceFingerOpen(char* pPrinterName);
    bool IsDeviceFingerOpenForPrint(char* pPrinterName);
    bool OpenFinger(char* pPrinterName);
    bool CloseFinger(char* pPrinterName);
    bool CheckFinger(char* pPrinterName, char* userName);
    bool CancelFingerPrint(char* pPrinterName);
    int DeleteFinger(char* pPrinterName, char* userName);
    int DeleteFingers(char* pPrinterName);

    int AddFinger(char* pPrinterName, int mode, char* userName, int *ret_index);
    bool ImportFinger(char* pPrinterName, char* fileName);
    int ExportFinger(char* pPrinterName, char* fileName, int *pdwLen);
    bool IsFingerPrint(char* pPrinterName, char* userName, short* pIndex);




private:
    bool abort;
    DeviceManager devicemanager;
#if SUPPORT_FINGERMANAGER
    CupsManager cupsmanager;
#endif
    FingCmd cmd;
};



#endif // FINGERCOMMON_H
