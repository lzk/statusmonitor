#include "FingCmd.h"
#include <unistd.h>

#include <cups/cups.h>
#include <cups/backend.h>
#include <cups/sidechannel.h>
#ifndef Q_OS_MAC
#include <sys/statfs.h>
#endif
#include <sys/time.h>


FingCmd::FingCmd()
{
    m_pDeviceIO = NULL;

    m_pfgData = NULL;
    m_bFingerPrint = false;
    m_bIsFingerPrintCancel = false;
}


FingCmd::FingCmd(char* pPrinterName)
{
    m_pDeviceIO = NULL;

    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    m_pfgData = NULL;
    m_bFingerPrint = false;
    m_bIsFingerPrintCancel = false;

}

FingCmd::FingCmd(char* pPrinterName, bool bPrint)
{
    m_pDeviceIO = NULL;

    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    m_pfgData = NULL;
    m_bFingerPrint = bPrint;
    m_bIsFingerPrintCancel = false;

}

void FingCmd::SetFingCmdPrinter(char* pPrinterName)
{
    m_pDeviceIO = NULL;
    LOGLOG("####FM:SetFingCmdPrinter: %s",pPrinterName);
    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    if(m_pDeviceIO == NULL)
        LOGLOG("####FM:SetFingCmdPrinter: m_pDeviceIO = NULL");


    m_pfgData = NULL;
    m_bFingerPrint = false;
    m_bIsFingerPrintCancel = false;
    waitCMDreturn = false;

}

void FingCmd::SetFingCmdPrinter(char* pPrinterName, bool bPrint)
{
    m_pDeviceIO = NULL;

    LOGLOG("####FM:SetFingCmdPrinter: %s",pPrinterName);
    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    LOGLOG("####FM:SetFingCmdPrinter: 1");

    m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    LOGLOG("####FM:SetFingCmdPrinter: 2");


    m_pfgData = NULL;
    m_bFingerPrint = bPrint;
    m_bIsFingerPrintCancel = false;
    waitCMDreturn =false;
    LOGLOG("####FM:SetFingCmdPrinter: exit");


}


FingCmd::~FingCmd()
{
    m_pDeviceIO = NULL;

    if(m_pfgData != NULL)
    {
        free(m_pfgData);
        m_pfgData = NULL;
    }
    m_bIsFingerPrintCancel = false;

}

int FingCmd::DataTransfer(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput)
{
    static char buffMax[MAX_SIZE_BUFF];
    static const char INIT_VALUE = 0xfe;
    //TCHAR		symbolicname[MAX_PATH];
    int nResult = _ACK;
    int nCount = 0;
    bool bWriteSuccess = false;

    LOGLOG("####FM:DataTransfer");

    int dwActualSize = 0;

    dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
    if(dwActualSize > 0)
    {
        int nErrCnt = 0;
        memset(buffMax, INIT_VALUE, sizeof(buffMax));

        while((dwActualSize = m_pDeviceIO->read(buffMax, sizeof(buffMax))) >=0)
        {
            if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
            {
                if(nErrCnt%5 == 0)
                    LOGLOG("####FM:get sync info");
                if(20 < nErrCnt++)
                {
                    nResult = _SW_USB_READ_TIMEOUT;
                    break;
                }
                //QThread.msleep(100);
                usleep(100000);
            }
            else if (dwActualSize > 0 && INIT_VALUE != buffMax[0]) // return info for setting command
            {
                FG_STA_T sta;

                LOGLOG("####FM:Get return command");
                memcpy((unsigned char *)&sta, buffMax, 8);

                //LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                if (sta.code == I3('STA'))
                {
                    //LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);
                    if(sta.ack == 'A')
                    {
                        LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                        if(sta.length > 0)
                        {
                            LOGLOG("####FM:printer is ready, and data length is %d", sta.length);
                        }
                        nResult = _ACK;
                        bWriteSuccess = true;
                        *pbResult = sta.status;
                        *pcbLen  = sta.length;
                    }
                    else
                    {
                        if(sta.ack == 'N')
                        {
                            LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                            if (20 < nErrCnt++)
                            {
                                nResult = _SW_USB_READ_TIMEOUT;
                                break;
                            }
                            nResult = _Printer_Not_Ready;
                            //QThread.msleep(100);
                            usleep(100000);
                            continue;
                        }
                        else
                        {
                            LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack)
                            if(sta.ack == 'B')
                                 nResult = _Printer_busy;
                            else
                                 nResult = _Printer_error;
                        }
                    }
                }
                else
                {
                    nResult = _SW_USB_DATA_FORMAT_ERROR;
                }
                LOGLOG("####FM:DataTransfer(): get data success.");
                break;
            }
            else
            {
                LOGLOG("####FM:Get command error!");
                nResult = _SW_USB_ERROR_OTHER;
                break;
            }
        }

        if (_ACK == nResult
            && NULL != pOutput
            && 0 < cbOutput)
            //&& sizeof(buffMax) >= cbOutput)
        {
            int cbRead = 0;

            if(cbOutput >=512)
            {
                nErrCnt = 0;
                memset(buffMax, INIT_VALUE, sizeof(buffMax));

                while((dwActualSize = m_pDeviceIO->read(buffMax, sizeof(buffMax))) >=0)
                {

                    if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
                    {
                        if(nErrCnt%5==0)
                           LOGLOG("####FM:get sync info");
                        if (20 < nErrCnt++)
                        {
                            nResult = _SW_USB_READ_TIMEOUT;
                            break;
                        }
        //              QThread.msleep(100);
                        usleep(100000);
                    }
                    else if (dwActualSize > 0 && dwActualSize<= 8 && INIT_VALUE != buffMax[0]) // return info for setting command
                    {
                        FG_STA_T sta;

                       // LOGLOG("####FM:Get return command");
                        memcpy((unsigned char *)&sta, buffMax, 8);

                        LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                        if (sta.code == I3('STA'))
                        {
                            LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);

                            if(sta.ack == 'A')
                            {
                                LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                                nResult = _ACK;

                            }
                            else
                            {
                                if(sta.ack == 'N')
                                {
                                    LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                                    if (20 < nErrCnt++)
                                    {
                                        nResult = _SW_USB_READ_TIMEOUT;
                                        break;
                                    }
                                    nResult = _Printer_Not_Ready;
                                    //QThread.msleep(100);
                                    usleep(100000);
                                    continue;
                                }
                                else
                                {
                                    LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack)
                                    if(sta.ack == 'B')
                                         nResult = _Printer_busy;
                                    else
                                         nResult = _Printer_error;
                                }
                            }
                        }
                        else
                        {
                            nResult = _SW_USB_DATA_FORMAT_ERROR;
                        }
                    }
                    else if (dwActualSize >= 512 && INIT_VALUE != buffMax[0])    // return info for getting command
                    {
                        LOGLOG("####FM:Get finger data, the size is %d", dwActualSize);
                        cbRead +=dwActualSize;
                        memcpy(pOutput+cbRead, buffMax, dwActualSize);
                        if(cbRead < cbOutput)
                        {
                            LOGLOG("####FM:Get Output is not complete");
                            continue;
                        }
                        nResult = _ACK;
                        break;
                    }
                    else
                    {
                        LOGLOG("####FM:Get command error!");
                        nResult = _SW_USB_ERROR_OTHER;
                        break;
                    }
                }

            }
            else
            {
                if(dwActualSize > 8)
                {
                    nResult = _ACK;
                    memcpy(pOutput, buffMax+8, cbOutput);
                    LOGLOG("####FM:WriteDataViaUSB(): wirte data 2.");
                }
            }

            if(nResult == _ACK)
            {
                bWriteSuccess = true;
                LOGLOG("####FM:WriteDataViaUSB(): wirte data success.");
            }


        }

    }
    else
    {
        nResult = _SW_USB_WRITE_TIMEOUT;
        LOGLOG("####FM:WriteDataViaUSB(): write usb timeout.");
    }


//        if(!bWriteSuccess)
//        {
////            QThread.msleep(200);
//            usleep(200000);
//        }


   return nResult;

}

int FingCmd::DataTransferForPrint(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput)
{
    static char buffMax[MAX_SIZE_BUFF];
    static const char INIT_VALUE = 0xfe;
    //TCHAR		symbolicname[MAX_PATH];
    int nResult = _ACK;
    int nCount = 0;
    bool bWriteSuccess = false;

    LOGLOG("####FM:DataTransfer");

    int dwActualSize = 0;

    dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
    if(dwActualSize > 0)
    {
        int nErrCnt = 0;
        memset(buffMax, INIT_VALUE, sizeof(buffMax));
        int readSize = 0;
        bool usbType = false;
        if(m_pDeviceIO->type() == m_pDeviceIO->Type_usb)
        {
            readSize = sizeof(buffMax);
            usbType = true;
        }
        else
        {
            readSize = sizeof(FG_STA_T);
        }

        while((dwActualSize = m_pDeviceIO->read(buffMax, readSize)) >=0 && !m_bIsFingerPrintCancel)
        {
            if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
            {
                if(nErrCnt%5 == 0)
                    LOGLOG("####FM:get sync info");
                if(20 < nErrCnt++)
                {
                    nResult = _SW_USB_READ_TIMEOUT;
                    break;
                }
                //QThread.msleep(100);
                usleep(100000);
            }
            else if (dwActualSize > 0 && INIT_VALUE != buffMax[0]) // return info for setting command
            {
                FG_STA_T sta;

                LOGLOG("####FM:Get return command");
                memcpy((unsigned char *)&sta, buffMax, 8);

                //LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                if (sta.code == I3('STA'))
                {
                    //LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);
                    if(sta.ack == 'A')
                    {
                        LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                        if(sta.length > 0)
                        {
                            LOGLOG("####FM:printer is ready, and data length is %d", sta.length);
                        }
                        nResult = _ACK;
                        bWriteSuccess = true;
                        *pbResult = sta.status;
                        *pcbLen  = sta.length;
                    }
                    else
                    {
                        if(sta.ack == 'N')
                        {
                            LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                            if (20 < nErrCnt++)
                            {
                                nResult = _SW_USB_READ_TIMEOUT;
                                break;
                            }
                            nResult = _Printer_Not_Ready;
                            //QThread.msleep(100);
                            usleep(100000);
                            continue;
                        }
                        else
                        {
                            LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack)
                            if(sta.ack == 'B')
                                 nResult = _Printer_busy;
                            else
                                 nResult = _Printer_error;
                        }
                    }
                }
                else
                {
                    nResult = _SW_USB_DATA_FORMAT_ERROR;
                }
                LOGLOG("####FM:DataTransfer(): get data success.");
                break;
            }
            else
            {
                LOGLOG("####FM:Get command error!");
                nResult = _SW_USB_ERROR_OTHER;
                break;
            }
        }

        if(m_bIsFingerPrintCancel)
        {
            nResult = _Printer_Cancel;
        }

        if (_ACK == nResult
            && NULL != pOutput
            && 0 < cbOutput)
            //&& sizeof(buffMax) >= cbOutput)
        {
            int cbRead = 0;

            if((cbOutput >=512 && usbType) || !usbType)
            {
                nErrCnt = 0;
                memset(buffMax, INIT_VALUE, sizeof(buffMax));

                if(usbType)
                {
                    readSize = sizeof(buffMax);
                }
                else
                {
                    readSize = sizeof(usbType);
                }

                while((dwActualSize = m_pDeviceIO->read(buffMax, readSize)) >=0 && !m_bIsFingerPrintCancel)
                {
                    if(!usbType)
                    {
                        if(dwActualSize >0)
                        {
                            nResult = _ACK;
                            bWriteSuccess = true;
                            memcpy(pOutput, buffMax, dwActualSize);
                            LOGLOG("####FM:WriteDataViaNet(): read data success!");
                        }
                        else
                        {
                            LOGLOG("####FM:WriteDataViaNet(): data format error!");
                            nResult = _SW_NET_DATA_FORMAT_ERROR;
                            usleep(200000);
                        }
                    }

                    else if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
                    {
                        if(nErrCnt%5==0)
                           LOGLOG("####FM:get sync info");
                        if (20 < nErrCnt++)
                        {
                            nResult = _SW_USB_READ_TIMEOUT;
                            break;
                        }
        //              QThread.msleep(100);
                        usleep(100000);
                    }
                    else if (dwActualSize > 0 && dwActualSize<= 8 && INIT_VALUE != buffMax[0]) // return info for setting command
                    {
                        FG_STA_T sta;

                       // LOGLOG("####FM:Get return command");
                        memcpy((unsigned char *)&sta, buffMax, 8);

                        LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                        if (sta.code == I3('STA'))
                        {
                            LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);

                            if(sta.ack == 'A')
                            {
                                LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                                nResult = _ACK;

                            }
                            else
                            {
                                if(sta.ack == 'N')
                                {
                                    LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                                    if (20 < nErrCnt++)
                                    {
                                        nResult = _SW_USB_READ_TIMEOUT;
                                        break;
                                    }
                                    nResult = _Printer_Not_Ready;
                                    //QThread.msleep(100);
                                    usleep(100000);
                                    continue;
                                }
                                else
                                {
                                    LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack)
                                    if(sta.ack == 'B')
                                         nResult = _Printer_busy;
                                    else
                                         nResult = _Printer_error;
                                }
                            }
                        }
                        else
                        {
                            nResult = _SW_USB_DATA_FORMAT_ERROR;
                        }
                    }
                    else if (dwActualSize >= 512 && INIT_VALUE != buffMax[0])    // return info for getting command
                    {
                        LOGLOG("####FM:Get finger data, the size is %d", dwActualSize);
                        cbRead +=dwActualSize;
                        memcpy(pOutput+cbRead, buffMax, dwActualSize);
                        if(cbRead < cbOutput)
                        {
                            LOGLOG("####FM:Get Output is not complete");
                            continue;
                        }
                        nResult = _ACK;
                        break;
                    }
                    else
                    {
                        LOGLOG("####FM:Get command error!");
                        nResult = _SW_USB_ERROR_OTHER;
                        break;
                    }
                }

                if(m_bIsFingerPrintCancel)
                {
                    nResult = _Printer_Cancel;
                }


            }
            else
            {
                if(dwActualSize > 8)
                {
                    nResult = _ACK;
                    memcpy(pOutput, buffMax+8, cbOutput);
                    LOGLOG("####FM:WriteDataViaUSB(): wirte data 2.");
                }
            }

            if(nResult == _ACK)
            {
                bWriteSuccess = true;
                LOGLOG("####FM:WriteDataViaUSB(): wirte data success.");
            }


        }

    }
    else
    {
        nResult = _SW_USB_WRITE_TIMEOUT;
        LOGLOG("####FM:WriteDataViaUSB(): write usb timeout.");
    }


//        if(!bWriteSuccess)
//        {
////            QThread.msleep(200);
//            usleep(200000);
//        }


   return nResult;

}

int FingCmd::WriteDataViaUSB(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput)
{
    static char buffMax[MAX_SIZE_BUFF];
    static const char INIT_VALUE = 0xfe;
    //TCHAR		symbolicname[MAX_PATH];
    int nResult = _ACK;
    int nCount = 0;
    bool bWriteSuccess = false;
    while(nCount++ < 10 && !bWriteSuccess)
    {
        LOGLOG("####FM:WriteDataViaUSB");
        if(m_bIsFingerPrintCancel && m_bFingerPrint)
        {
            LOGLOG("####FM:WriteDataViaUSB: cancel");
            nResult = _Printer_Cancel;
            break;
        }
//        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//        {

            int dwActualSize = 0;
            //nResult = m_pDeviceIO->open();
            nResult = m_pDeviceIO->open(&mPrinter, 0);
            if(!nResult)
            {
                 LOGLOG("####FM:WriteDataViaUSB(): usb open.");
                dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
                if(dwActualSize > 0)
                {
                   int nErrCnt = 0;
                   memset(buffMax, INIT_VALUE, sizeof(buffMax));

                  while((dwActualSize = m_pDeviceIO->read(buffMax, sizeof(buffMax))) >=0)
                  {
                      if(m_bIsFingerPrintCancel &&  m_bFingerPrint)
                      {
                          LOGLOG("####FM:WriteDataViaUSB: cancel 2");
                          nResult = _Printer_Cancel;
                          break;
                      }
                       if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
                       {
                            LOGLOG("####FM:get sync info");
                            if(20 < nErrCnt++)
                            {
                                nResult = _SW_USB_READ_TIMEOUT;
                                break;
                            }
                            //QThread.msleep(100);
                            usleep(100000);
                        }
                        else if (dwActualSize > 0 && INIT_VALUE != buffMax[0]) // return info for setting command
                        {
                            FG_STA_T sta;

                            LOGLOG("####FM:Get return command");
                            memcpy((unsigned char *)&sta, buffMax, 8);

                            LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                            if (sta.code == I3('STA'))
                            {
                                LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);
                                if(sta.ack == 'A')
                                {
                                    LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                                    nResult = _ACK;
                                    bWriteSuccess = true;
                                    *pbResult = sta.status;
                                    *pcbLen  = sta.length;
                                }
                                else
                                {
                                    if(sta.ack == 'N')
                                    {
                                        LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                                        if (20 < nErrCnt++)
                                        {
                                            nResult = _SW_USB_READ_TIMEOUT;
                                            break;
                                        }
                                        nResult = _Printer_Not_Ready;
                                        //QThread.msleep(100);
                                        usleep(100000);
                                        continue;
                                    }
                                    else
                                    {
                                        if(sta.ack == 'B')
                                        {
                                            nResult = _Printer_busy;
                                            LOGLOG("####FM:WriteDataViaUSB:  get Busy");
                                            bWriteSuccess = true;
                                            break;
                                        }
                                        else
                                            nResult = _Printer_error;
                                    }
                                }
                                LOGLOG("####FM:WriteDataViaUSB:  get data success. dwActualSize=%d", dwActualSize);
                            }
                            else
                            {
                                nResult = _SW_USB_DATA_FORMAT_ERROR;
                                LOGLOG("####FM:WriteDataViaUSB:  Get command error. dwActualSize=%d", dwActualSize);
                            }
                            
                            break;
                        }
                        else
                        {
                            LOGLOG("####FM:Get command error!");
                            nResult = _SW_USB_ERROR_OTHER;
                            break;
                        }
                  }

                    if (_ACK == nResult
                        && NULL != pOutput
                        && 0 < cbOutput)
                        //&& sizeof(buffMax) >= cbOutput)
                    {
                        int cbRead = 0;

                        if(cbOutput >=512)
                        {
                            nErrCnt = 0;
                            memset(buffMax, INIT_VALUE, sizeof(buffMax));

                            while((dwActualSize = m_pDeviceIO->read(buffMax, sizeof(buffMax))) >=0)
                            {
                                if(m_bIsFingerPrintCancel && m_bFingerPrint)
                                {
                                    LOGLOG("####FM:WriteDataViaUSB: cancel 3");
                                    nResult = _Printer_Cancel;
                                    break;
                                }

                                if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
                                {
                                    LOGLOG("####FM:get sync info");
                                    if (20 < nErrCnt++)
                                    {
                                        nResult = _SW_USB_READ_TIMEOUT;
                                        break;
                                    }
    //                                   QThread.msleep(100);
                                    usleep(100000);
                                }
                                else if (dwActualSize > 0 && dwActualSize<= 8 && INIT_VALUE != buffMax[0]) // return info for setting command
                                {
                                    FG_STA_T sta;

                                    LOGLOG("####FM:Get return command");
                                    memcpy((unsigned char *)&sta, buffMax, 8);

                                    LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                                    if (sta.code == I3('STA'))
                                    {
                                        LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);

                                        if(sta.ack == 'A')
                                        {
                                            LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                                            nResult = _ACK;

                                        }
                                        else
                                        {
                                            if(sta.ack == 'N')
                                            {
                                                LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                                                if (20 < nErrCnt++)
                                                {
                                                    nResult = _SW_USB_READ_TIMEOUT;
                                                    break;
                                                }
                                                nResult = _Printer_Not_Ready;
                                                //QThread.msleep(100);
                                                usleep(100000);
                                                continue;
                                            }
                                            else
                                            {
                                                LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack)
                                                if(sta.ack == 'B')
                                                     nResult = _Printer_busy;
                                                else
                                                     nResult = _Printer_error;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        nResult = _SW_USB_DATA_FORMAT_ERROR;
                                    }
                                }
                                else if (dwActualSize >= 512 && INIT_VALUE != buffMax[0])    // return info for getting command
                                {
                                    cbRead +=dwActualSize;
                                    memcpy(pOutput+cbRead, buffMax, dwActualSize);
                                    if(cbRead < cbOutput)
                                    {
                                        LOGLOG("####FM:Get Output is not complete");
                                        continue;

                                    }
                                        nResult = _ACK;
                                        break;
                                }
                                else
                                {
                                    LOGLOG("####FM:Get command error!");
                                    nResult = _SW_USB_ERROR_OTHER;
                                    break;
                                }
                            }

                        }
                        else
                        {
                            if(dwActualSize > 8)
                            {
                                nResult = _ACK;
                                memcpy(pOutput, buffMax+8, cbOutput);
                                LOGLOG("####FM:WriteDataViaUSB(): wirte data 2.");
                            }
                        }

                        if(nResult == _ACK)
                        {
                            bWriteSuccess = true;
                            LOGLOG("####FM:WriteDataViaUSB(): wirte data success.");
                        }

                    }

                }
                else
                {
                    nResult = _SW_USB_WRITE_TIMEOUT;
                    LOGLOG("####FM:WriteDataViaUSB(): write usb timeout.");
                }
                m_pDeviceIO->close();
                LOGLOG("####FM:WriteDataViaUSB(): usb close.");
            }
            else
            {
                nResult = _SW_USB_OPEN_FAIL;
                LOGLOG("####FM:WriteDataViaUSB(): open usb fail.");
            }
//        }
//        else
//        {

//        }
        if(!bWriteSuccess)
        {
//            QThread.msleep(200);
            usleep(200000);
        }

    }
   return nResult;

}

int FingCmd::WriteDataViaUSBwithCUPS(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput)
{
    static char buffMax[MAX_SIZE_BUFF];
    static const char INIT_VALUE = 0xfe;
    //TCHAR		symbolicname[MAX_PATH];
    int nResult = _ACK;
    int nCount = 0;
    bool bWriteSuccess = false;
    int needRead = *pcbLen;
    if(needRead == 0)
    {
        needRead = MAX_SIZE_BUFF;
    }
    int timeout_cnt = 0;
    LOGLOG("####FM:WriteDataViaUSBwithCUPS start");
    while(nCount++ < 20 && !bWriteSuccess)
    {
        LOGLOG("####FM:WriteDataViaUSBwithCUPS nCount = %d", nCount);
        if(m_bIsFingerPrintCancel && m_bFingerPrint)
        {
            LOGLOG("####FM:WriteDataViaUSBwithCUPS: cancel");
            nResult = _Printer_Cancel;
            break;
        }


        int dwActualSize = 0;
        int readOffset = 0;
        int readedSize = 0;

        LOGLOG("####FM:WriteDataViaUSBwithCUPS(): write cmd...");

        //dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
        dwActualSize = fwrite(pInput, cbInput, 1, stdout);
        LOGLOG("####FM:WriteDataViaUSBwithCUPS(): writed cmd. dwActualSize=%d", dwActualSize);
        /* Flush pending output to stdout */
        fflush(stdout);
        cups_sc_status_t sc_status = CUPS_SC_STATUS_OK;
        int buff_len =MAX_SIZE_BUFF;

        sc_status = cupsSideChannelDoRequest(CUPS_SC_CMD_DRAIN_OUTPUT, buffMax, &buff_len, 15.0);

            LOGLOG("####FM:WriteDataViaUSBwithCUPS(): status=%d, gdatalen=%d\n",sc_status, buff_len);
        if(sc_status == CUPS_SC_STATUS_OK)
        {
            //timeout_cnt = 0;
            int nErrCnt = 0;
            int errorDataCnt = 0;
            memset(buffMax, INIT_VALUE, sizeof(buffMax));
            while((dwActualSize = cupsBackChannelRead(buffMax, sizeof(buffMax), 30.0)) >=0)
//            while((dwActualSize = cupsBackChannelRead(buffMax+readOffset, needRead - readOffset, 30.0)) >=0)
            {
//                LOGLOG("####FM:WriteDataViaUSBwithCUPS: *pcbLen=%d",*pcbLen);

//                if(*pcbLen !=0)
//                {
//                    readOffset += dwActualSize;
//                    if(readOffset < needRead)
//                    {
//                        LOGLOG("####FM:WriteDataViaUSBwithCUPS: read data length enouth! continue");
//                        continue;
//                    }
//                }
                if(m_bIsFingerPrintCancel &&  m_bFingerPrint)
                {
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: cancel 2");
                    nResult = _Printer_Cancel;
                    break;
                }
                if(dwActualSize >=8)
                {
                    char *p_STA;
                    p_STA = strstr(buffMax, "STA");
                    if(p_STA == NULL)
                    {
                        nResult = _SW_USB_DATA_FORMAT_ERROR;
                        break;
//                        errorDataCnt ++;
//                        //LOGLOG("####FM:WriteDataViaUSBwithCUPS:  receive data is: %s", buffMax);
//                        LOGLOG("####FM:WriteDataViaUSBwithCUPS:  receive data not contain STA");
//                        if(errorDataCnt >10 )
//                        {
//                            errorDataCnt=0;
//                            break;
//                        }
//                        usleep(10000);
//                        continue;
                    }
                }
                else
                {
                    errorDataCnt ++;

                    nResult = _SW_USB_DATA_FORMAT_ERROR;
                    break;
//                    LOGLOG("####FM:WriteDataViaUSBwithCUPS:  Get command error. dwActualSize=%d", dwActualSize);
//                    if(errorDataCnt > 10)
//                    {
//                         errorDataCnt=0;
//                        break;
//                    }
//                    usleep(10000);
//                    continue;
                }

                if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
                {
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: get sync info");
                    if(20 < nErrCnt++)
                    {
                        nResult = _SW_USB_READ_TIMEOUT;
                        break;
                    }
                    //QThread.msleep(100);
                    usleep(100000);
                }
                else if (dwActualSize > 0 && INIT_VALUE != buffMax[0]) // return info for setting command
                {
                    FG_STA_T sta;

                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: Get return command");
                    memcpy((unsigned char *)&sta, buffMax, 8);
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                    if (sta.code == I3('STA'))
                    {
                        LOGLOG("####FM:WriteDataViaUSBwithCUPS: Recevie return command, and ack is %c", sta.ack);
                        if(sta.ack == 'A')
                        {
                            LOGLOG("####FM:WriteDataViaUSBwithCUPS: printer is ready, and result is %d, sta.len=%d", sta.status, sta.length);
                            nResult = _ACK;
                            bWriteSuccess = true;
                            *pbResult = sta.status;
                            *pcbLen  = sta.length;
                        }
                        else
                        {
                            if(sta.ack == 'N')
                            {
                                LOGLOG("####FM:WriteDataViaUSBwithCUPS: Printer is not ready, wait for 20*100 ms");
                                if (20 < nErrCnt++)
                                {
                                    nResult = _SW_USB_READ_TIMEOUT;
                                    break;
                                }
                                nResult = _Printer_Not_Ready;
                                //QThread.msleep(100);
                                usleep(100000);
                                continue;
                            }
                            else
                            {
                                if(sta.ack == 'B')
                                {
                                    nResult = _Printer_busy;
                                    LOGLOG("####FM:WriteDataViaUSBwithCUPS:  get Busy");
                                    bWriteSuccess = true;
                                    break;

                                }
                                else
                                    nResult = _Printer_error;
                            }
                        }
                        LOGLOG("####FM:WriteDataViaUSBwithCUPS:  get data success. dwActualSize=%d", dwActualSize);

                    }
                    else
                    {
                        nResult = _SW_USB_DATA_FORMAT_ERROR;
                        LOGLOG("####FM:WriteDataViaUSBwithCUPS:  Get command error. dwActualSize=%d", dwActualSize);

                    }
                    break;
                }
                else
                {
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: Get command error!");
                    nResult = _SW_USB_ERROR_OTHER;
                    break;
                }
            }

            if(dwActualSize < 0)
            {
                LOGLOG("####FM:WriteDataViaUSBwithCUPS: Receive Data fail! dwActualSize=%d, try again",dwActualSize);
                usleep(100000);
                nResult = _SW_USB_WRITE_TIMEOUT;
                continue;
            }

            if (_ACK == nResult
                && NULL != pOutput
                && 0 < cbOutput)
                //&& sizeof(buffMax) >= cbOutput)
            {
                int cbRead = 0;
#if 0
                if(cbOutput >=512)
                {
                    nErrCnt = 0;
                    memset(buffMax, INIT_VALUE, sizeof(buffMax));

                    while((dwActualSize = m_pDeviceIO->read(buffMax, sizeof(buffMax))) >=0)
                    {
                        if(m_bIsFingerPrintCancel && m_bFingerPrint)
                        {
                            LOGLOG("####FM:WriteDataViaUSB: cancel 3");
                            nResult = _Printer_Cancel;
                            break;
                        }
                        if (buffMax[0] == 0x1C && buffMax[1] == 0x2D) // sync info
                        {
                            LOGLOG("####FM:get sync info");
                            if (20 < nErrCnt++)
                            {
                                nResult = _SW_USB_READ_TIMEOUT;
                                break;
                            }
    //                      QThread.msleep(100);
                            usleep(100000);
                        }
                        else if (dwActualSize > 0 && dwActualSize<= 8 && INIT_VALUE != buffMax[0]) // return info for setting command
                        {
                            FG_STA_T sta;
                            LOGLOG("####FM:Get return command");
                            memcpy((unsigned char *)&sta, buffMax, 8);
                            LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                            if (sta.code == I3('STA'))
                            {
                                LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack);
                                if(sta.ack == 'A')
                                {
                                    LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                                    nResult = _ACK;
                                }
                                else
                                {
                                    if(sta.ack == 'N')
                                    {
                                        LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                                        if (20 < nErrCnt++)
                                        {
                                            nResult = _SW_USB_READ_TIMEOUT;
                                            break;
                                        }
                                        nResult = _Printer_Not_Ready;
                                        //QThread.msleep(100);
                                        usleep(100000);
                                        continue;
                                    }
                                    else
                                    {
                                        LOGLOG("####FM:Recevie return command, and ack is %c", sta.ack)
                                        if(sta.ack == 'B')
                                            nResult = _Printer_busy;
                                        else
                                            nResult = _Printer_error;
                                    }
                                }
                            }
                            else
                            {
                                nResult = _SW_USB_DATA_FORMAT_ERROR;
                            }
                        }
                        else if (dwActualSize >= 512 && INIT_VALUE != buffMax[0])    // return info for getting command
                        {
                            cbRead +=dwActualSize;
                            memcpy(pOutput+cbRead, buffMax, dwActualSize);
                            if(cbRead < cbOutput)
                            {
                                LOGLOG("####FM:Get Output is not complete");
                                continue;
                            }
                            nResult = _ACK;
                            break;
                        }
                        else
                        {
                            LOGLOG("####FM:Get command error!");
                            nResult = _SW_USB_ERROR_OTHER;
                            break;
                        }
                    }

                }
                else
#endif
                {
                    if(dwActualSize > 8)
                    {
                        nResult = _ACK;
                        memcpy(pOutput, buffMax+8, cbOutput);
                        LOGLOG("####FM:WriteDataViaUSBwithCUPS(): wirte data 2.");
//                        for(int i=0; i< dwActualSize; i++)
//                        {
//                            LOGLOG("####FM:WriteDataViaUSBwithCUPS(): data[%d]=%d.", i, buffMax[i]);

//                        }
                    }
                    else if(dwActualSize == 8)
                    {
                        if((dwActualSize = cupsBackChannelRead(buffMax, sizeof(buffMax), 30.0)) >0)
                        {

                            LOGLOG("####FM:WriteDataViaUSBwithCUPS(): wirte data 3, dwActualSize=%d.",dwActualSize);
                            FG_STA_T sta;
                            memcpy((unsigned char *)&sta, buffMax, 8);
                            if (sta.code == I3('STA'))
                            {
                                LOGLOG("####FM:WriteDataViaUSBwithCUPS: Recevie return command, and ack is %c", sta.ack);
                                if(sta.ack == 'A')
                                {
                                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: printer is ready, and result is %d", sta.status);
                                    nResult = _ACK;
                                    bWriteSuccess = true;
                                    *pbResult = sta.status;
                                    *pcbLen  = sta.length;
                                    memcpy(pOutput, buffMax+8, cbOutput);
                                }
                                else
                                {
                                    if(sta.ack == 'N')
                                    {
                                        LOGLOG("####FM:WriteDataViaUSBwithCUPS: Printer is not ready, wait for 20*100 ms");
                                        if (20 < nErrCnt++)
                                        {
                                            nResult = _SW_USB_READ_TIMEOUT;
                                            break;
                                        }
                                        nResult = _Printer_Not_Ready;
                                        //QThread.msleep(100);
                                        usleep(100000);
                                        continue;
                                    }
                                    else
                                    {
                                        if(sta.ack == 'B')
                                            nResult = _Printer_busy;
                                        else
                                            nResult = _Printer_error;
                                    }
                                }
                            }
                            else
                            {
                                nResult = _SW_USB_DATA_FORMAT_ERROR;
                            }



                        }
                    }
                }
                if(nResult == _ACK)
                {
                    bWriteSuccess = true;
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS(): wirte data success.");
                }
            }
        }
        else
        {
            if(sc_status == CUPS_SC_STATUS_IO_ERROR)
            {
                nResult = _SW_USB_WRITE_TIMEOUT;
                LOGLOG("####FM:WriteDataViaUSBwithCUPS: CUPS_SC_STATUS_IO_ERROR: sc_status=%d", sc_status);
                break;
            }
            else if(sc_status == CUPS_SC_STATUS_TIMEOUT)
            {
                nResult = _SW_USB_WRITE_TIMEOUT;
                LOGLOG("####FM:WriteDataViaUSBwithCUPS(): write usb timeout.");

            }
            else if(sc_status == CUPS_SC_STATUS_NO_RESPONSE)
            {
                nResult = _SW_USB_WRITE_TIMEOUT;
                LOGLOG("####FM:WriteDataViaUSBwithCUPS: CUPS_SC_STATUS_BAD_MESSAGE: sc_status=%d", sc_status);
            }
            else
            {
                nResult = _SW_USB_WRITE_TIMEOUT;
                LOGLOG("####FM:WriteDataViaUSBwithCUPS: other error: sc_status=%d", sc_status);


            }

        }



        if(!bWriteSuccess)
        {
//            QThread.msleep(200);
            usleep(200000);
        }

    }
   return nResult;

}

int FingCmd::WriteDataViaNet(BYTE* pInput, DWORD cbInput, BYTE *pOutput, WORD* pcbLen, BYTE* pbResult, WORD cbOutput)
{

    //static const char INIT_VALUE = 0xfe;
    char buffMax[MAX_SIZE_BUFF];
    memset(buffMax ,0xfe ,sizeof(buffMax));
    //TCHAR		symbolicname[MAX_PATH];
    int nResult = _SW_NET_CONNECT_FAIL;
    int nCount = 0;
    bool bWriteSuccess = false;
    while(nCount++ < 5 && !bWriteSuccess)
    {
        LOGLOG("####FM:WriteDataViaNet");
        if(m_bIsFingerPrintCancel && m_bFingerPrint)
        {
            LOGLOG("####FM:WriteDataViaNet: cancel");
            nResult = _Printer_Cancel;
            break;
        }

        int dwActualSize = 0;
        LOGLOG("####FM:WriteDataViaNet :open");
        LOGLOG("####FM:WriteDataViaNet:open:%s", mPrinter.deviceUri);


        nResult = m_pDeviceIO->open(&mPrinter, 10001);
        LOGLOG("####FM:WriteDataViaNet:opened:%d", nResult);

        if(!nResult)
        {
            LOGLOG("####FM:WriteDataViaNet :write");

            dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
            LOGLOG("####FM:WriteDataViaNet :wrote:%d", dwActualSize);

            if(dwActualSize > 0)
            {
                int nErrCnt = 0;
                FG_STA_T sta;

                //while((dwActualSize = m_pDeviceIO->read((char*)&sta, sizeof(FG_STA_T)+cbOutput)) >=0)
                while((dwActualSize = m_pDeviceIO->read(buffMax, sizeof(FG_STA_T)+cbOutput)) >=0)
                {
                    if(m_bIsFingerPrintCancel && m_bFingerPrint)
                    {
                        LOGLOG("####FM:WriteDataViaNet: cancel 2");
                        nResult = _Printer_Cancel;
                        break;
                    }
                    memcpy((unsigned char *)&sta, buffMax, 8);
                    //if (dwActualSize == sizeof(FG_STA_T) && sta.code == I3('STA'))
                    if (dwActualSize > 0 && sta.code == I3('STA'))
                    {

                        LOGLOG("####FM:return command is 0x%x, 0x%x, length is %d, ack is %c", I3('STA'), sta.code, sta.length, sta.ack);
                        if(sta.ack == 'A')
                        {
                            //LOGLOG("####FM:printer is ready, and result is %d", sta.status);
                            nResult = _ACK;
                            bWriteSuccess = true;
                            *pbResult = sta.status;
                            *pcbLen  = sta.length;
                         }
                         else
                         {
                            if(sta.ack == 'N')
                            {
                                LOGLOG("####FM:Printer is not ready, wait for 20*100 ms");
                                if (20 < nErrCnt++)
                                {
                                    nResult = _SW_USB_READ_TIMEOUT;
                                    break;
                                }
                                nResult = _Printer_Not_Ready;
//                              QThread.msleep(100);
                                usleep(100000);
                                continue;
                            }
                            else
                            {
                                if(sta.ack == 'B')
                                {
                                    nResult = _Printer_busy;
                                }
                                else
                                {
                                    nResult = _Printer_error;
                                    *pbResult = sta.status;
                                }
                            }
                        }
                        bWriteSuccess = true;
                        LOGLOG("####FM:WriteDataViaNet(): get data success.");
                        break;
                    }
                    else
                    {
                        LOGLOG("####FM:WriteDataViaNet(): data format error!");
                        nResult = _SW_NET_DATA_FORMAT_ERROR;
                        //break;
                    }
                }

                if ( NULL != pOutput
                        && 0 < cbOutput)
                        //&& sizeof(buffMax) >= cbOutput)
                {

                    //if((dwActualSize = m_pDeviceIO->read((char*)pOutput, cbOutput)) >0)
                    if(dwActualSize == cbOutput + 8)
                    {
                        memcpy(pOutput, buffMax+8, cbOutput);
                        nResult = _ACK;
                        bWriteSuccess = true;
                        LOGLOG("####FM:WriteDataViaNet(): read data success!");
                    }
                    else
                    {
                        LOGLOG("####FM:WriteDataViaNet(): data format error!");
                        nResult = _SW_NET_DATA_FORMAT_ERROR;
                        usleep(200000);
                    }



                }

            }
            else
            {
                nResult = _SW_USB_WRITE_TIMEOUT;
                LOGLOG("####FM:WriteDataViaNet(): write usb timeout.");
                usleep(200000);
            }
            m_pDeviceIO->close();
        }
        else
        {
            nResult = _SW_USB_OPEN_FAIL;
            LOGLOG("####FM:WriteDataViaNet(): connect fail.");
            usleep(200000);
            if(nCount > 2)
            {
                break;
            }
        }


    }
   return nResult;

}


int FingCmd::GetFingerStatus()
{
    FG_FUNC_T fgCmd;
    BYTE result;
    WORD length;
    WORD cbRead=0;
    int nResult;
    BYTE cmd[4] = {'F','I','N','G'};

    LOGLOG("####FM: GetFingerStatus");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_FUNC_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.mode = 'R';

    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
    {
        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);
    }
    else
    {
        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);

    }
    if(nResult == _ACK)
    {
        return result;
    }
    return -1;
}

int FingCmd::GetFingerStatusForPrint()
{
    FG_FUNC_T fgCmd;
    BYTE result;
    WORD length = 8;
    WORD cbRead=0;
    int nResult;
    BYTE cmd[4] = {'F','I','N','G'};

    LOGLOG("####FM: GetFingerStatusForPrint");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_FUNC_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.mode = 'R';

    //if(m_pDeviceIO->type() == DeviceIO::Type_usb)
    if(devicemanager.getDeviceType(mPrinter.deviceUri) == DeviceIO::Type_usb)
    {
        LOGLOG("####FM: GetFingerStatusForPrint U");

        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);
//        int trytime = 0;
//        do
//        {
//            nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);
//            LOGLOG("####FM: GetFingerStatusForPrint U try %d times", trytime);
//            trytime++;

//        }while(trytime <=3 && nResult == _SW_USB_DATA_FORMAT_ERROR);

    }
    else
    {
        LOGLOG("####FM: GetFingerStatusForPrint N");

        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);

    }
     if(nResult == _ACK)
    {
        return result;
    }
    return -1;
}

bool FingCmd::SetFingerStatus(unsigned char status)
{
    FG_FUNC_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'F','I','N','G'};

    LOGLOG("####FM: SetFingerStatus");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_FUNC_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.mode = 'W';
    fgCmd.func = status;
    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
    {
        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);
    }
    else
    {
        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);

    }
    if(nResult == _ACK)
    {
        return true;
    }
    return false;
}

int FingCmd::Check(char* userName, int len)
{
    FG_CHECK_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result=0;
    WORD length = 0;
    BYTE cmd[4] = {'C','H','C','K'};
    unsigned char* pData = NULL;

    LOGLOG("####FM:: Check");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_CHECK_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.length = len;

    pData = (unsigned char*)malloc(sizeof(FG_CHECK_T) +len+2);
    if(pData != NULL)
    {
        DWORD dwSize = sizeof(FG_CHECK_T) +len;

        memcpy(pData, &fgCmd, sizeof(FG_CHECK_T));
        memcpy(pData+sizeof(FG_CHECK_T), (unsigned char*)userName, len);
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
        {
            nResult = WriteDataViaUSB((unsigned char*)pData, dwSize, NULL, &length, &result, cbRead);

         }
        else
        {
            nResult = WriteDataViaNet((unsigned char*)pData, dwSize, NULL, &length, &result, cbRead);

        }

        if(nResult == _ACK)
        {

            if(result == 0)
                nResult = _User_Not_Exist;

        }

        free(pData);
        pData = NULL;


    }
    return nResult;
}

bool FingCmd::CancelPrint()
{
    FG_CANCEL_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'C','A','N','L'};

    LOGLOG("####FM: CancelPrint");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_CANCEL_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
    {
        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_CANCEL_T), NULL, &length, &result, cbRead);

        //    nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd, sizeof(FG_CANCEL_T), NULL, &length, &result, cbRead);
    }
    else
    {
        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_CANCEL_T), NULL, &length, &result, cbRead);
    }
    if(nResult == _ACK)
    {
        return true;
    }
    return false;
}

bool FingCmd::CancelPrintWithTrans()
{
    FG_CANCEL_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'C','A','N','L'};

    LOGLOG("####FM: CancelPrint");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_CANCEL_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_CANCEL_T), NULL, &length, &result, cbRead);

    if(nResult == _ACK)
    {
        return true;
    }
    return false;
}

int FingCmd::Delete(BYTE mode, char* userName, int len)
{
    FG_DELETE_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'D','E','L','E'};

    LOGLOG("####FM: Delete");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_DELETE_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.mode = mode;

    if(mode == 0)
    {
        fgCmd.length = len;

        unsigned char* pData = NULL;
        pData = (unsigned char*)malloc(sizeof(FG_DELETE_T) +len+2);
        if(pData != NULL)
        {
            DWORD dwSize = sizeof(FG_DELETE_T) +len;

            memcpy(pData, &fgCmd, sizeof(FG_DELETE_T));
            memcpy(pData+sizeof(FG_DELETE_T), (unsigned char*)userName, len);
            if(m_pDeviceIO->type() == DeviceIO::Type_usb)
            {
                nResult = WriteDataViaUSB((unsigned char*)pData, dwSize, NULL, &length, &result, cbRead);
            }
            else
            {
                nResult = WriteDataViaNet((unsigned char*)pData, dwSize, NULL, &length, &result, cbRead);
            }
            free(pData);
            pData = NULL;

            if(nResult == _Printer_error)
            {
                nResult = _User_Not_Exist;
            }


        }
    }
    else
    {
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
        {
            nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_DELETE_T), NULL, &length, &result, cbRead);
        }
        else
        {
            nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_DELETE_T), NULL, &length, &result, cbRead);
        }

    }


    return nResult;
}

int FingCmd::IsPrint(char* userName, short* pIndex, int mTimeout)
{
    FG_DISC_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'D','I','S','C'};

    LOGLOG("####FM: IsPrint");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_DISC_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    //length = 8;
#if OPEN_ONE
    bool bOpenSuccess = false;
    int nCount = 0;
    while(nCount++ < 1 && !bOpenSuccess)
    {
        if(m_bIsFingerPrintCancel)
        {
            break;
        }
        //if(m_pDeviceIO->type() == DeviceIO::Type_usb)
         if(devicemanager.getDeviceType(mPrinter.deviceUri) == DeviceIO::Type_usb)
        {
            //nResult = m_pDeviceIO->open();
            nResult = m_pDeviceIO->open(&mPrinter);
        }
        else
        {
            //nResult = m_pDeviceIO->open(10001);
            nResult = m_pDeviceIO->open(&mPrinter, 10001);
        }
        if(!nResult)
        {
            bOpenSuccess = true;
            break;
        }
        else
        {
            nResult = _SW_USB_OPEN_FAIL;
            LOGLOG("####FM:IsPrint(): open usb fail.");
        }
    }
#endif

    if(m_bIsFingerPrintCancel)
    {
#if OPEN_ONE
        CancelPrintWithTrans();
         if(bOpenSuccess)
         {
             m_pDeviceIO->close();
         }
#else
        CancelPrint();
#endif
         return _Printer_Cancel;
    }
#if OPEN_ONE
    if(!bOpenSuccess)
        return nResult;


    //nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
    nResult = DataTransferForPrint((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
#else
    LOGLOG("####FM:IsPrint():  DISC cmd ....");

    bool usbType = false;
    //if(m_pDeviceIO->type() == DeviceIO::Type_usb)

    if(devicemanager.getDeviceType(mPrinter.deviceUri) == DeviceIO::Type_usb)
    {
        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
        //nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
        usbType = true;
    }
    else
    {
        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
    }

#endif
    if(nResult == _ACK)
    {
        int count = mTimeout *10 /3+4;
        FG_STATUS_T fgCmd1;
        BYTE cmd1[4] = {'S','T','U','S'};

        fgCmd1.code = *(unsigned int*)&cmd1[0];
        //length = 8;
        struct timeval tpstart, tpend;/*for test time*/
        gettimeofday(&tpstart, 0);
        while(count--&& !m_bIsFingerPrintCancel)
        {
            gettimeofday(&tpend, 0);
            float timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
            timeuse /= 1000000;
            if(timeuse -mTimeout > 0 )
            {

                CancelPrint();
                break;
            }
#if OPEN_ONE
            //nResult = DataTransfer((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
            nResult = DataTransferForPrint((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
#else
            LOGLOG("####FM:IsPrint():  STUS cmd ....");

            if(usbType)
            {
                nResult = WriteDataViaUSB((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
                //nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
            }
            else
            {
                nResult = WriteDataViaNet((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
            }
            LOGLOG("####FM:IsPrint(%d):  STUS cmd end", count);
#endif
            if(nResult == _ACK ) //&& result > 0)
            {
                if(result == 1)
                {
                    FG_GETRESULT_T fgCmd2;
                    usleep(500000);
                    BYTE cmd2[4] = {'G','E','T','R'};
                    unsigned char* pData;
                    FG_DATA_T fgData;

                    fgCmd2.code = *(unsigned int*)&cmd2[0];
                    pData = (unsigned char*)&fgData;

                    cbRead = sizeof(FG_DATA_T);
                    //length = sizeof(FG_DATA_T) +10;

                    memset(pData, 0, cbRead);

#if OPEN_ONE
                    nResult = DataTransferForPrint((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
#else
                    LOGLOG("####FM:IsPrint():  GETR cmd ....");

                    if(usbType)
                    {
                        nResult = WriteDataViaUSB((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
                        //nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);

                    }
                    else
                    {
                        nResult = WriteDataViaNet((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
                    }
                     LOGLOG("####FM:IsPrint():  GETR cmd end nResult=%d, length=%d", nResult, length);
#endif
                    if(nResult == _ACK  && length > 0)
                    {

#if OPEN_ONE
                        if(bOpenSuccess)
                        {
                            m_pDeviceIO->close();
                        }
#endif
                        //memcpy((unsigned char*)&fgData, pData, length*sizeof(FG_DATA_T));
                        if(strlen((const char*)fgData.UserName) > 0)
                        {
                            strcpy(userName, (const char*)fgData.UserName);
                            *pIndex = fgData.ID;
                            LOGLOG("####FM:IsPrint():  success:UserName=%s, id=%d", userName, fgData.ID);
                            //free(pData);
                        }
                        else
                        {
                             LOGLOG("####FM:IsPrint():  GETR userName is 0, id=%d", fgData.ID);
                            return _Printer_Finger_Wrong;
                        }

                        return _ACK;
                    }
                    else if(nResult == _ACK  && length ==0)
                    {
                         LOGLOG("####FM:IsPrint(): length ==0 fail try again!");

                    }


                }
                else if(result > 1)
                {
#if OPEN_ONE
                    if(bOpenSuccess)
                    {
                        m_pDeviceIO->close();
                    }
#endif

                    return _Printer_Finger_Wrong;
                }

//                else
//                {
//                    if(bOpenSuccess)
//                    {
//                        m_pDeviceIO->close();
//                    }
//                    return _Printer_Finger_Wrong;
//                }
            }
            else
            {
                if(nResult == _Printer_busy)
                {
                    //return _Printer_busy;
                    LOGLOG("####FM:IsPrint(): STUS busy, try again!");

                }
            }
            usleep(333400);
        }

        //CancelPrintWithTrans();
    }
    else
    {
        if(nResult == _Printer_busy)
        {
            return _Printer_busy;
        }
        nResult = _Printer_error;
    }

    if(m_bIsFingerPrintCancel)
    {
#if OPEN_ONE
        CancelPrintWithTrans();
         if(bOpenSuccess)
         {
             m_pDeviceIO->close();
         }
#else
        CancelPrint();
#endif
         return _Printer_Cancel;
    }
#if OPEN_ONE
    if(bOpenSuccess)
    {
        m_pDeviceIO->close();
    }
#endif
    if(nResult == _Printer_busy)
    {
        return _Printer_busy;
    }
    return _Printer_Time_Out;
}

int FingCmd::Import(FG_DATAF_T* pfgData, int len)
{
    FG_IMPORT_T fgCmd;
    WORD cbRead=0, cbWritten = 0;
    int nResult = _Printer_Time_Out;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'I','M','P','T'};


    bool bOpenSuccess = false;
    int nCount = 0;
    while(nCount++ < 5 && !bOpenSuccess)
    {
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
        {
            //nResult = m_pDeviceIO->open();
            nResult = m_pDeviceIO->open(&mPrinter, 0);
        }
        else
        {
            //nResult = m_pDeviceIO->open(10001);
            nResult = m_pDeviceIO->open(&mPrinter, 10001);
        }
        if(!nResult)
        {
            bOpenSuccess = true;
            break;
        }
        else
        {
            nResult = _SW_USB_OPEN_FAIL;
            LOGLOG("####FM:IsPrint(): open usb fail.");
        }
    }
    if(!bOpenSuccess)
        return false;//nResult;

    unsigned char* pData = NULL;
    //pData = (unsigned char*)malloc(len*sizeof(FG_DATAF_T)+2);

    LOGLOG("####FM: Import");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_IMPORT_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.num = len;


        nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_IMPORT_T), NULL, &length, &result,  0);

        if(nResult == _ACK)
        {
            DWORD dwSize = len*sizeof(FG_DATAF_T);
            pData = (unsigned char*)malloc(len*sizeof(FG_DATAF_T)+8);
            if(pData != NULL)
            {
                int count = 3;
                while(count--)
                {

                    memcpy(pData, & fgCmd, sizeof(FG_IMPORT_T));
                    memcpy(pData+sizeof(FG_IMPORT_T), pfgData, len*sizeof(FG_DATAF_T));
                    DWORD writeSize = dwSize+8;
                    DWORD writeSizePerTime = dwSize+8;
                    DWORD wroteSize = 0;
                    while(writeSize>0)
                    {
                        writeSizePerTime = writeSize;
                        if(writeSizePerTime > 4096)
                            writeSizePerTime = 4096;

                        cbWritten = m_pDeviceIO->write_bulk((char*)pData+wroteSize, writeSizePerTime);
                        if(cbWritten > 0)
                        {
                            wroteSize +=cbWritten;
                            writeSize -=cbWritten;
                        }
                        if(cbWritten == 0)
                        {
                            break;
                        }
                    }

                   // if(cbWritten == len*sizeof(FG_DATAF_T)+8)
                    if(wroteSize == dwSize+8)
                    {
                        LOGLOG("####FM: Import success");
                        nResult = _ACK;
                        break;
                    }
                    else
                        sleep(1);
                }
                if(pData != NULL)
                {
                    free(pData);
                    pData = NULL;
                }
            }

               // CancelPrintWithTrans();


        }


    if(bOpenSuccess)
    {
        m_pDeviceIO->close();
    }

    return nResult;//_Printer_Time_Out;
}


int FingCmd::Export(int* len)
{
    FG_EXPORT_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length =0;
    BYTE cmd[4] = {'E','X','P','T'};

    LOGLOG("####CFingCmd: Export");

    memset((unsigned char*)&fgCmd, 0, sizeof(FG_EXPORT_T));

    fgCmd.code = *(unsigned int*)&cmd[0];

    LOGLOG("Send Export command!");
    if(m_pDeviceIO->type() != DeviceIO::Type_usb)
    {
        return -1;
    }



    //    nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_EXPORT_T), NULL, &length, &result, cbRead);

    //    LOGLOG("nResult(%d) = %d",_ACK, nResult);
    bool bOpenSuccess = false;
    int nCount = 0;
    while(nCount++ < 5 && !bOpenSuccess)
    {
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
        {
            //nResult = m_pDeviceIO->open();
            nResult = m_pDeviceIO->open(&mPrinter, 0);
        }
        else
        {
            //nResult = m_pDeviceIO->open(10001);
            nResult = m_pDeviceIO->open(&mPrinter, 10001);
        }
        if(!nResult)
        {
            bOpenSuccess = true;
            break;
        }
        else
        {
            nResult = _SW_USB_OPEN_FAIL;
            LOGLOG("####FM:Export(): open usb fail.");
        }
    }
    if(!bOpenSuccess)
        return nResult;
        nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_EXPORT_T), NULL, &length, &result, cbRead);

        LOGLOG("nResult(%d) = %d",_ACK, nResult);

    if(nResult == _ACK)
    {
        int count = 6;
        FG_STA_T sta;
        DWORD dwRead = 0;
        usleep(100000);
        while(count--)
        {
            dwRead = m_pDeviceIO->read_bulk((char*)&sta, 8);

            LOGLOG("Get Return command, and dwRead is %d", dwRead);
            if(dwRead > 0)
            {
                LOGLOG("sta code is %x, the expt is %x", sta.code, I4('EXPT'));
                if(sta.code==I4('EXPT'))
                {
                    int  flength = sta.length;
                    unsigned char* pData;


                    result = 0;
                    if(m_pfgData != NULL)
                    {
                        free(m_pfgData);
                    }
                    m_pfgData = (FG_DATAF_T*)malloc(sizeof(FG_DATAF_T)*flength);
                    if(m_pfgData != NULL)
                    {
                        pData = (unsigned char*)m_pfgData;
                        DWORD needRead = sizeof(FG_DATAF_T)*flength;
                        int readPerTime = 0;
                        int readed = 0;
                        int zeroTime = 0;
                        LOGLOG("Send GetResult Command!");
                        while(needRead > 0)
                        {
                            //dwRead =  m_pDeviceIO->read_bulk((char*)pData, cbRead);
                            readPerTime = needRead;
                            if(readPerTime > 4096)
                            {
                                readPerTime = 4096;
                            }
                            dwRead =  m_pDeviceIO->read_bulk((char*)pData+readed, readPerTime);
                            if(dwRead)
                            {
                                readed += dwRead;
                                needRead -= dwRead;
                                zeroTime = 0;
                            }
                            else
                            {
                                zeroTime++;
                                if(zeroTime > 2)
                                {
                                    LOGLOG("read data 0 = %d!", zeroTime);
                                    break;

                                }
                            }


                        }

                        if(readed > 0)
                        {
                            *len = flength;
                            nResult = _ACK;
                            LOGLOG("read data success!");
                        }
                        else
                        {
                            nResult = _Get_parameter_error;
                        }
                    }

                    if(bOpenSuccess)
                    {
                        m_pDeviceIO->close();
                    }
                    return nResult;
                }

            }
            sleep(1);
        }
        CancelPrintWithTrans();

    }
    if(bOpenSuccess)
    {
        m_pDeviceIO->close();
    }

    return _Printer_Time_Out;
}

int FingCmd::GetFingerData(FG_DATAF_T* pfgData, int len)
{
    if(m_pfgData != NULL)
    {
        memcpy((unsigned char*)pfgData, (unsigned char*)m_pfgData, sizeof(FG_DATAF_T)*len);
        free(m_pfgData);
        m_pfgData = NULL;

        return len;
    }

    return 0;
}

int FingCmd::AddFingerFirst(char* userName)
{
    FG_ADD_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'A','D','D','F'};
    int len = strlen(userName);

    LOGLOG("####FM:: AddFingerFirst");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_ADD_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.length = strlen(userName);

#if OPEN_ONE
    bool bOpenSuccess = false;
    int nCount = 0;
    while(nCount++ < 5 && !bOpenSuccess)
    {
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
        {
            //nResult = m_pDeviceIO->open();
            nResult = m_pDeviceIO->open(&mPrinter, 0);
        }
        else
        {
            //nResult = m_pDeviceIO->open(10001);
            nResult = m_pDeviceIO->open(&mPrinter, 10001);
        }
        if(!nResult)
        {
            bOpenSuccess = true;
            break;
        }
        else
        {
            nResult = _SW_USB_OPEN_FAIL;
            LOGLOG("####FM:AddFingerFirst(): open usb fail.");
        }
    }
    if(!bOpenSuccess)
        return nResult;
#endif

    unsigned char* pData = NULL;
    pData = (unsigned char*)malloc(sizeof(FG_ADD_T) + len );
    if(pData != NULL)
    {
        DWORD dwSize = sizeof(FG_ADD_T) +len*sizeof(FG_DATA_T);

        memcpy(pData, &fgCmd, sizeof(FG_ADD_T));
        memcpy(pData+sizeof(FG_ADD_T), (unsigned char*)userName, len);
#if OPEN_ONE
        nResult = DataTransfer((unsigned char*)pData, sizeof(FG_ADD_T)+len, NULL, &length, &result, cbRead);
#else
//        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//        {
                nResult = WriteDataViaUSB((unsigned char*)pData, sizeof(FG_ADD_T)+len, NULL, &length, &result, cbRead);
//        }
//        else
//        {
//                nResult = WriteDataViaNet((unsigned char*)pData, sizeof(FG_ADD_T)+len, NULL, &length, &result, cbRead);
//         }
#endif
        if(nResult == _ACK)
        {
            int count = 30;
            FG_STATUS_T fgCmd1;
            BYTE cmd1[4] = {'S','T','U','S'};

            fgCmd1.code = *(unsigned int*)&cmd1[0];

            while(count--)
            {
#if OPEN_ONE
                nResult = DataTransfer((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
#else
//                if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//                {
                        nResult = WriteDataViaUSB((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
//                }
//                else
//                {
//                        nResult = WriteDataViaNet((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
//                 }
#endif
                LOGLOG("return is %d, status is %d", nResult,  result);
                if(nResult == _ACK && result > 0)
                {
                    if( result == 1)
                    {
                        FG_GETRESULT_T fgCmd2;
                        BYTE cmd2[4] = {'G','E','T','R'};
                        fgCmd2.code = *(unsigned int*)&cmd2[0];
#if OPEN_ONE
                        nResult = DataTransfer((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), NULL, &length, &result, 0);
                        if(bOpenSuccess)
                        {
                            m_pDeviceIO->close();
                        }
#else
//                        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//                        {
                                nResult = WriteDataViaUSB((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), NULL, &length, &result, 0);
//                        }
//                        else
//                        {
//                                nResult = WriteDataViaNet((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), NULL, &length, &result, 0);
//                         }
#endif
                        if(pData != NULL)
                        {
                            free(pData);
                            pData = NULL;
                        }
                        return nResult;
                    }
                    else
                    {
#if OPEN_ONE
                        if(bOpenSuccess)
                        {
                            m_pDeviceIO->close();
                        }
#endif
                        if(pData != NULL)
                        {
                            free(pData);
                            pData = NULL;
                        }
                        LOGLOG("Add fail!");
                        return _Get_parameter_error;
                    }
                }

                sleep(1);
            }
#if OPEN_ONE
            CancelPrintWithTrans();
#else
            CancelPrint();
#endif
        }
        if(pData != NULL)
        {
            free(pData);
            pData = NULL;
        }
    }
#if OPEN_ONE
    if(bOpenSuccess)
    {
        m_pDeviceIO->close();
    }
#endif
    return nResult;
}


int FingCmd::AddFingerSecond(char* userName, short* pIndex)
{
    FG_ADD_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'A','D','D','S'};

    LOGLOG("####FM: AddFingerSecond");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_ADD_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
#if OPEN_ONE
    bool bOpenSuccess = false;
    int nCount = 0;
    while(nCount++ < 5 && !bOpenSuccess)
    {
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
        {
            //nResult = m_pDeviceIO->open();
            nResult = m_pDeviceIO->open(&mPrinter, 0);
        }
        else
        {
            //nResult = m_pDeviceIO->open(10001);
            nResult = m_pDeviceIO->open(&mPrinter, 10001);
        }
        if(!nResult)
        {
            bOpenSuccess = true;
            break;
        }
        else
        {
            nResult = _SW_USB_OPEN_FAIL;
            LOGLOG("####FM:AddFingerSecond(): open usb fail.");
        }
    }
    if(!bOpenSuccess)
        return nResult;


    nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_ADD_T), NULL, &length, &result, cbRead);
#else
//    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//    {
        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_ADD_T), NULL, &length, &result, cbRead);
//    }
//    else
//    {
//        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_ADD_T), NULL, &length, &result, cbRead);
//    }
    LOGLOG("####FM:AddFingerSecond(): ADDS return is %d.", nResult);
#endif
    if(nResult == _ACK)
    {
        int count = 30;
        FG_STATUS_T fgCmd1;
        BYTE cmd1[4] = {'S','T','U','S'};

        fgCmd1.code = *(unsigned int*)&cmd1[0];

        while(count--)
        {
#if OPEN_ONE
            nResult = DataTransfer((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
#else
//            if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//            {
                nResult = WriteDataViaUSB((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
//            }
//            else
//            {
//                nResult = WriteDataViaNet((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
//            }
              LOGLOG("####FM:AddFingerSecond(): STUS return is %d.", nResult);
#endif
            if(nResult == _ACK )
            {
                LOGLOG("####FM:AddFingerSecond(): STUS return result=%d, length=%d.", result, length);
                if(result == 1 /*&& length >= 1*/)
                //if(length >= 1) // only for test FW V00.06.xx
                {
                    FG_GETRESULT_T fgCmd2;
                    BYTE cmd2[4] = {'G','E','T','R'};
                    FG_DATA_T fgData;

                    fgCmd2.code = *(unsigned int*)&cmd2[0];
                    memset(&fgData, 0, sizeof(FG_DATA_T));
#if OPEN_ONE
                    nResult = DataTransfer((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), (unsigned char*)&fgData, &length, &result, sizeof(FG_DATA_T));
#else
//                    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//                    {
                        nResult = WriteDataViaUSB((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), (unsigned char*)&fgData, &length, &result, sizeof(FG_DATA_T));
//                    }
//                    else
//                    {
//                        nResult = WriteDataViaNet((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), (unsigned char*)&fgData, &length, &result, sizeof(FG_DATA_T));
//                    }
                        LOGLOG("####FM:AddFingerSecond(): GETR return is %d.", nResult);
#endif
                    if(nResult == _ACK && length != 0)
                    {
                        if(strlen((const char*)fgData.UserName) <= 0)
                        {
                            LOGLOG("####FM:AddFingerSecond():  GETR userName is 0");
                            return _Finger_Error;
                        }
                        else
                        {
                            //strcpy(userName, (const char*)fgData.UserName);
                            if(strlen((const char*)fgData.UserName) >= 32)
                                memcpy(userName, (const char*)fgData.UserName, sizeof(fgData.UserName));
                            else
                                strcpy(userName, (const char*)fgData.UserName);
                            *pIndex = fgData.ID;
                             LOGLOG("####FM:AddFingerSecond(): success.");
                             LOGLOG("####FM:AddFingerSecond(): id=%d, name=%s.", fgData.ID, fgData.UserName);
                        }
                    }
#if OPEN_ONE
                    if(bOpenSuccess)
                    {
                        m_pDeviceIO->close();
                    }
#endif
                    return nResult;

                }
                else
                {

#if OPEN_ONE
                    if(bOpenSuccess)
                    {
                        m_pDeviceIO->close();
                    }
#endif
                    if(result == 2)
                    {
                        LOGLOG("####FM:AddFingerSecond(): result is 2.");
                        return _Finger_Error;
                    }
                    //return nResult;//_Get_parameter_error;
                }
            }

            sleep(1);

        }
#if OPEN_ONE
        CancelPrintWithTrans();
#else
        CancelPrint();
#endif
    }
#if OPEN_ONE
    if(bOpenSuccess)
    {
        m_pDeviceIO->close();
    }
#endif
    LOGLOG("####FM:AddFingerSecond(): add fail.");
    return _Finger_Error;//_Printer_Time_Out;
}

