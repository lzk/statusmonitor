#ifndef FINGCMD_H
#define FINGCMD_H

#include "jkinterface.h"
#include "commonapi.h"
#include "datadefine.h"
#include "common.h"

#define OPEN_ONE 0

class FingCmd
{

public:
    FingCmd();
    FingCmd(char* pPrinterName);
    FingCmd(char* pPrinterName, bool bPrint);
    ~FingCmd();

private:
    //CDeviceIO* m_pDeviceIO;
    DeviceIO* m_pDeviceIO;
    DeviceManager devicemanager;
    Printer_struct mPrinter;
    int WriteDataViaUSB(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput);
    int WriteDataViaNet(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput);
    int WriteDataViaUSBwithCUPS(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput);
    int DataTransfer(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput);
    int DataTransferForPrint(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput);
    bool CancelPrintWithTrans();


public:
    FG_DATAF_T* m_pfgData;

    bool	m_bFingerPrint;
    bool    m_bIsFingerPrintCancel;

    int GetFingerStatus();
    int GetFingerStatusForPrint();

    bool SetFingerStatus(unsigned char status);

    bool Check(char* userName, int len);

    bool CancelPrint();

    int Delete(BYTE mode, char* userName, int len);

    int IsPrint(char* userName, short* pIndex);

    bool Import(FG_DATAF_T* pfgData, int len);

    int Export(int* len);

    int GetFingerData(FG_DATAF_T* pfgData, int len);

    int AddFingerFirst(char* userName);

    int AddFingerSecond(char* userName, short* pIndex);

    void SetFingCmdPrinter(char* pPrinterName);
    void SetFingCmdPrinter(char* pPrinterName, bool bPrint);


};

#endif // FINGCMD_H
