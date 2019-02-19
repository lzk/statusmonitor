#include "FingCmd.h"
#include <unistd.h>

#include <cups/cups.h>
#include <cups/backend.h>
#include <cups/sidechannel.h>


FingCmd::FingCmd()
{
    m_pDeviceIO = NULL;

    m_pfgData = NULL;
    m_bFingerPrint = FALSE;
    m_bIsFingerPrintCancel = false;
}


FingCmd::FingCmd(char* pPrinterName)
{
    m_pDeviceIO = NULL;

    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    DeviceIO* m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    m_pfgData = NULL;
    m_bFingerPrint = FALSE;
    m_bIsFingerPrintCancel = false;

}

FingCmd::FingCmd(char* pPrinterName, bool bPrint)
{
    m_pDeviceIO = NULL;

    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    DeviceIO* m_pDeviceIO = devicemanager.getDevice(&mPrinter);
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
    DeviceIO* m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    if(m_pfgData != NULL)
    {
        free(m_pfgData);
    }
    m_pfgData = NULL;
    m_bFingerPrint = false;
    m_bIsFingerPrintCancel = false;

}

void FingCmd::SetFingCmdPrinter(char* pPrinterName, bool bPrint)
{
    m_pDeviceIO = NULL;

    LOGLOG("####FM:SetFingCmdPrinter: %s",pPrinterName);
    //m_pDeviceIO = devicemanager.getDevice(pPrinterName);
    strcpy(mPrinter.deviceUri, pPrinterName);
    DeviceIO* m_pDeviceIO = devicemanager.getDevice(&mPrinter);
    if(m_pfgData != NULL)
    {
        free(m_pfgData);
    }
    m_pfgData = NULL;
    m_bFingerPrint = bPrint;
    m_bIsFingerPrintCancel = false;

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
                            LOGLOG("####FM:WriteDataViaUSB(): get data success.");
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
    while(nCount++ < 10 && !bWriteSuccess)
    {
        LOGLOG("####FM:WriteDataViaUSBwithCUPS");
        if(m_bIsFingerPrintCancel && m_bFingerPrint)
        {
            LOGLOG("####FM:WriteDataViaUSBwithCUPS: cancel");
            nResult = _Printer_Cancel;
            break;
        }


        int dwActualSize = 0;

        LOGLOG("####FM:WriteDataViaUSBwithCUPS(): write cmd...");

        //dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
        dwActualSize = fwrite(pInput, cbInput, 1, stdout);
        LOGLOG("####FM:WriteDataViaUSBwithCUPS(): writed cmd. dwActualSize=%d", dwActualSize);
        /* Flush pending output to stdout */
        fflush(stdout);
        cups_sc_status_t sc_status = CUPS_SC_STATUS_OK;
        int buff_len =MAX_SIZE_BUFF;
        //sc_status = cupsSideChannelDoRequest(CUPS_SC_CMD_DRAIN_OUTPUT, buffMax, &buff_len, 30.0);
        LOGLOG("####FM:WriteDataViaUSBwithCUPS(): status=%d, gdatalen=%d\n",sc_status, buff_len);
        if(sc_status == CUPS_SC_STATUS_OK)
        {
            int nErrCnt = 0;
            memset(buffMax, INIT_VALUE, sizeof(buffMax));

            while((dwActualSize = cupsBackChannelRead(buffMax, sizeof(buffMax), 30.0)) >=0)
            {
                if(m_bIsFingerPrintCancel &&  m_bFingerPrint)
                {
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS: cancel 2");
                    nResult = _Printer_Cancel;
                    break;
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
                            LOGLOG("####FM:WriteDataViaUSBwithCUPS: printer is ready, and result is %d", sta.status);
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
                    LOGLOG("####FM:WriteDataViaUSBwithCUPS:  get data success.");
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
            nResult = _SW_USB_WRITE_TIMEOUT;
            LOGLOG("####FM:WriteDataViaUSBwithCUPS(): write usb timeout.");
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
    char buffMax[MAX_SIZE_BUFF] = {0xfe};
    //TCHAR		symbolicname[MAX_PATH];
    int nResult = _ACK;
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
        nResult = m_pDeviceIO->open(&mPrinter, 10001);
        if(!nResult)
        {
            dwActualSize = m_pDeviceIO->write((char*)pInput, cbInput);
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
        if(result == 0)
            return 0;
        else
            return 1;
    }
    return -1;
}

int FingCmd::GetFingerStatusForPrint()
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
        //nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);
        nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);

    }
    else
    {
        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_FUNC_T), NULL, &length, &result, cbRead);

    }
    if(nResult == _ACK)
    {
        if(result == 0)
            return 0;
        else
            return 1;
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

bool FingCmd::Check(char* userName, int len)
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
        free(pData);
        pData = NULL;

        if(nResult == _ACK)
        {
            if(result == 0)
                return false;
            else
                return true;
        }
    }
    return false;
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
        if(m_bFingerPrint)
            nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd, sizeof(FG_CANCEL_T), NULL, &length, &result, cbRead);
         else
            nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_CANCEL_T), NULL, &length, &result, cbRead);
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

int FingCmd::IsPrint(char* userName, short* pIndex)
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
#if OPEN_ONE
    bool bOpenSuccess = false;
    int nCount = 0;
    while(nCount++ < 5 && !bOpenSuccess )
    {
        if(m_bIsFingerPrintCancel)
        {
            break;
        }
        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
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
    bool usbType = false;
    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
    {
//        nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
        nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
        usbType = true;
    }
    else
    {
        nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_DISC_T), NULL, &length, &result, cbRead);
    }

#endif
    if(nResult == _ACK)
    {
        int count = 10;
        FG_STATUS_T fgCmd1;
        BYTE cmd1[4] = {'S','T','U','S'};

        fgCmd1.code = *(unsigned int*)&cmd1[0];

        while(count--&& !m_bIsFingerPrintCancel)
        {
#if OPEN_ONE
            //nResult = DataTransfer((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
            nResult = DataTransferForPrint((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result,  cbRead);
#else
            if(usbType)
            {
//                nResult = WriteDataViaUSB((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
                nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
            }
            else
            {
                nResult = WriteDataViaNet((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
            }
#endif
            if(nResult == _ACK ) //&& result > 0)
            {
                if(result == 1)
                {
                    FG_GETRESULT_T fgCmd2;
                    BYTE cmd2[4] = {'G','E','T','R'};
                    unsigned char* pData;
                    FG_DATA_T fgData;

                    fgCmd2.code = *(unsigned int*)&cmd2[0];
                    pData = (unsigned char*)&fgData;

                    cbRead = sizeof(FG_DATA_T);
                    memset(pData, 0, cbRead);
#if OPEN_ONE
                    nResult = DataTransferForPrint((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
#else
                    if(usbType)
                    {
//                        nResult = WriteDataViaUSB((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
                        nResult = WriteDataViaUSBwithCUPS((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);

                    }
                    else
                    {
                        nResult = WriteDataViaNet((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
                    }
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
                            //free(pData);
                        }
                        else
                        {
                             LOGLOG("####FM:IsPrint():  GETR userName is 0");
                            return _Printer_Finger_Wrong;
                        }

                        return _ACK;
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
            usleep(5000000);
        }
        //CancelPrintWithTrans();
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
    return _Printer_Time_Out;
}

bool FingCmd::Import(FG_DATAF_T* pfgData, int len)
{
    FG_IMPORT_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'I','M','P','T'};

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
            LOGLOG("####FM:IsPrint(): open usb fail.");
        }
    }
    if(!bOpenSuccess)
        return false;//nResult;
#endif

    unsigned char* pData = NULL;
    pData = (unsigned char*)malloc(len*sizeof(FG_DATAF_T)+2);

    LOGLOG("####FM: Import");
    memset((unsigned char*)&fgCmd, 0, sizeof(FG_IMPORT_T));

    fgCmd.code = *(unsigned int*)&cmd[0];
    fgCmd.num = len;

    if(pData != NULL)
    {
        DWORD dwSize = len*sizeof(FG_DATA_T);

        memcpy(pData, &fgCmd, sizeof(FG_IMPORT_T));
        memcpy(pData+sizeof(FG_IMPORT_T), (unsigned char*)pfgData, len*sizeof(FG_DATAF_T));
#if OPEN_ONE
        nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_IMPORT_T), NULL, &length, &result,  0);
#else
//        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//        {
            nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_IMPORT_T), NULL, &length, &result,  0);
//        }
//        else
//        {
//            nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_IMPORT_T), NULL, &length, &result,  0);
//        }
#endif
        if(nResult == _ACK)
        {
            //memcpy(pData, (unsigned char*)pfgData, len*sizeof(FG_DATAF_T));
            //nResult = m_pDeviceIO->WriteData(NULL, dwSize, pData, &length, &result, 0);

            //if(nResult == _ACK)
            //{
                int count = 30;
                FG_STATUS_T fgCmd1;
                BYTE cmd1[4] = {'S','T','U','S'};

                fgCmd1.code = *(unsigned int*)&cmd1[0];

                while(count--)
                {
#if OPEN_ONE
                    nResult = DataTransfer((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
#else
//                    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//                    {
                        nResult = WriteDataViaUSB((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
//                    }
//                    else
//                    {
//                        nResult = WriteDataViaNet((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
//                    }
#endif

                    if(nResult == _ACK && result > 0)
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
                        if(result == 1)
                        {
                            return true;//_ACK;
                        }
                        else
                        {
                            return false;//_Printer_error;
                        }
                    }
                    sleep(1);
                }

                //CancelPrint();
#if OPEN_ONE
                CancelPrintWithTrans();
#else
                CancelPrint();
#endif

            //}
                if(pData != NULL)
                {
                    free(pData);
                    pData = NULL;
                }
        }

    }
#if OPEN_ONE
    if(bOpenSuccess)
    {
        m_pDeviceIO->close();
    }
#endif
    return false;//_Printer_Time_Out;
}


int FingCmd::Export(int* len)
{
    FG_EXPORT_T fgCmd;
    WORD cbRead=0;
    int nResult;
    BYTE result;
    WORD length;
    BYTE cmd[4] = {'E','X','P','T'};

    LOGLOG("####CFingCmd: Export");

    memset((unsigned char*)&fgCmd, 0, sizeof(FG_EXPORT_T));

    fgCmd.code = *(unsigned int*)&cmd[0];

    LOGLOG("Send Export command!");
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
            LOGLOG("####FM:Export(): open usb fail.");
        }
    }
    if(!bOpenSuccess)
        return nResult;


    nResult = DataTransfer((unsigned char*)&fgCmd, sizeof(FG_EXPORT_T), NULL, &length, &result, cbRead);
#else
//    if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//    {
            nResult = WriteDataViaUSB((unsigned char*)&fgCmd, sizeof(FG_EXPORT_T), NULL, &length, &result, cbRead);
//    }
//    else
//    {
//            nResult = WriteDataViaNet((unsigned char*)&fgCmd, sizeof(FG_EXPORT_T), NULL, &length, &result, cbRead);
//     }
#endif
    if(nResult == _ACK)
    {
        int count = 30;
        FG_STATUS_T fgCmd1;
        BYTE cmd1[4] = {'S','T','U','S'};

        fgCmd1.code = *(unsigned int*)&cmd1[0];
        result = 0;
        length = 0;
        LOGLOG("Send Status command!");
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
//                    nResult = WriteDataViaNet((unsigned char*)&fgCmd1, sizeof(FG_STATUS_T), NULL, &length, &result, cbRead);
//             }
#endif
            LOGLOG("Get Return command, and result is %d", result);
            if(nResult == _ACK && result > 0)
            {
                LOGLOG("the finger nubmer is %d", length);
                if(result == 1 )//&& length >0)
                {
                    FG_GETRESULT_T fgCmd2;
                    BYTE cmd2[4] = {'G','E','T','R'};
                    unsigned char* pData;

                    length = 10;
                    fgCmd2.code = *(unsigned int*)&cmd2[0];
                    result = 0;
                    if(m_pfgData != NULL)
                    {
                        free(m_pfgData);
                    }
                    m_pfgData = (FG_DATAF_T*)malloc(sizeof(FG_DATAF_T)*length);
                    if(m_pfgData != NULL)
                    {
                        pData = (unsigned char*)m_pfgData;
                        cbRead = sizeof(FG_DATAF_T)*length;
                        LOGLOG("Send GetResult Command!");
#if OPEN_ONE
                        nResult = DataTransfer((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
#else
//                        if(m_pDeviceIO->type() == DeviceIO::Type_usb)
//                        {
                                nResult = WriteDataViaUSB((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
//                        }
//                        else
//                        {
//                                nResult = WriteDataViaNet(((unsigned char*)&fgCmd2, sizeof(FG_GETRESULT_T), pData, &length, &result, cbRead);
//                         }
#endif
                        if(nResult == _ACK)
                        {
                            //nResult = m_pDeviceIO->WriteData(NULL, 0, pData, &status, cbRead*sizeof(FG_DATAF_T));
                            //memcpy((unsigned char*)m_pfgData, pData, sizeof(FG_DATAF_T)*length);
                            *len = length;
                            LOGLOG("read data success!");
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
                    if(result == 1)
                    {
                        *len = 0;
                        return _ACK;
                    }
                    else
                    {
                        return _Get_parameter_error;
                    }
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

