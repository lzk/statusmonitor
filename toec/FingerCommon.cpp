#include "FingerCommon.h"
#include "commonapi.h"
#if SUPPORT_FINGERMANAGER
#include "smconfig.h"
#endif
#include <QDebug>
#include <QFile>

FingerCommon::FingerCommon()
{
#if SUPPORT_FINGERMANAGER
    printers.clear();
    printerlist.clear();
#endif
}

#if SUPPORT_FINGERMANAGER
static int callback_getPrinters(void* para,Printer_struct* ps)
{
    FingerCommon* st = (FingerCommon*)para;
    if(isDeviceSupported && isDeviceSupported(ps)){
        st->printers << *ps;
        st->printerlist << ps->name;
    }
    return 1;
}
#endif
////////////////////////////////////////////////////////////////////////////
//check the printer is installed
//return value:
// true: the printer is exist and installed
//false: the printer is not exist or installed
bool  FingerCommon::IsDeviceConnect(char *pPrinterName)
{
#if SUPPORT_FINGERMANAGER
    qDebug()<<"IsDeviceConnect ==>Enter";
    printers.clear();
    printerlist.clear();
    DeviceIO* device;
    cupsmanager.getPrinters(callback_getPrinters ,this);
    if(printers.size() >0)
    {
        foreach (Printer_struct printer, printers) {
            qDebug()<<"deviceUri="<<printer.deviceUri;
            qDebug()<<"makeAndModel="<<printer.makeAndModel;
            qDebug()<<"name="<<printer.name;
            //if(devicemanager.getDeviceType(printer.deviceUri) == DeviceIO::Type_usb)
            if(devicemanager.getDeviceType(&printer) == DeviceIO::Type_usb)
            {
                 //device = devicemanager.getDevice(printer.deviceUri);
                device = devicemanager.getDevice(&printer);
                 if(device->isConnected())
                 {
                     strcpy(pPrinterName, printer.deviceUri);
                     qDebug()<<"name="<<printer.name<< " is connect";
                     return true;
                 }
                 else
                 {
                     qDebug()<<"name="<<printer.name<< " is not connect";
                 }
            }
             else
             {
                     qDebug()<<"name="<<printer.name<< " is not USB";
                     if(device->isConnected())
                     {
                         //strcpy(pPrinterName, printer.deviceUri);
                         qDebug()<<"name="<<printer.name<< " is connect";
                         //return true;
                     }
                     else
                     {
                         qDebug()<<"name="<<printer.name<< " is not connect";
                     }
             }


            }
    }
#endif
    return false;
}

///////////////////////////////////////////////////////////////////
//check the printer is connected
bool FingerCommon::IsDeviceConnectForPrint(char *pPrinterName)
{

     //DeviceIO* device = devicemanager.getDevice(pPrinterName);
    Printer_struct mPrinter;
    strcpy(mPrinter.deviceUri, pPrinterName);
    DeviceIO* device = devicemanager.getDevice(&mPrinter);
     //if(device->isConnected())
    if(device->isConnected(&mPrinter))
     {


        int type = device->type();//devicemanager.getDeviceType(pPrinterName);
        if(type == DeviceIO::Type_usb || type == DeviceIO::Type_net)
        {

            return true;
        }
     }
    return false;
}

bool FingerCommon::getDevice()
{
#if SUPPORT_FINGERMANAGER
     DeviceIO* device;
     cupsmanager.getPrinters(callback_getPrinters ,this);
     foreach (Printer_struct printer, printers) {
         //device = devicemanager.getDevice(printer.deviceUri);
         device = devicemanager.getDevice(&printer);
         qDebug()<<"*****************************";
         qDebug()<<"connectTo="<<printer.connectTo;
         qDebug()<<"deviceUri="<<printer.deviceUri;
         qDebug()<<"isDefault="<<printer.isDefault;
         qDebug()<<"makeAndModel="<<printer.makeAndModel;
         qDebug()<<"name="<<printer.name;
//         result = getStatusFromDevice(device ,&status);
//         if(result){
//             LOGLOG("get status from device %s:fail!" ,printer.name);
//             memset(&status ,0 ,sizeof(status));
//             status.PrinterStatus = PS_UNKNOWN;
//         }else{
//             LOGLOG("get status from device %s:success!" ,printer.name);
//             LOGLOG("status:0x%02x" ,status.PrinterStatus);
//             if(IsStatusAbnormal(status.PrinterStatus)){
//                 status.PrinterStatus = PS_OFFLINE;
//             }
//         }

     }
#endif
}

//////////////////////////////////////////////////////////////////////////
//check whether the finger function is open
bool FingerCommon::IsDeviceFingerOpen(char* pPrinterName)
{
    LOGLOG("\r\n####FM: IsDeviceFingerOpen===>Enter");
    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);


    int nRet = -1;
    if((nRet = cmd.GetFingerStatus()) == 1)
    {
        return true;
    }

    return false;
}

bool FingerCommon::IsDeviceFingerOpenForPrint(char* pPrinterName)
{
    LOGLOG("\r\n####FM: IsDeviceFingerOpenForPrint===>Enter");
    LOGLOG("####FM:IsDeviceFingerOpenForPrint: %s",pPrinterName);
    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);

    int nRet = -1;
    nRet = cmd.GetFingerStatusForPrint();
    if(nRet == 1)
    {
        LOGLOG("\r\n####FM: the finger function is open");
        return true;
    }
    LOGLOG("\r\n####FM: the finger function is close");
    return false;
}

//////////////////////////////////////////////////////////////
//open the finger function
bool FingerCommon::OpenFinger(char* pPrinterName)
{
    LOGLOG("\r\n####FM: OpenFinger===Enter");
    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);

    return cmd.SetFingerStatus(1);
}

/////////////////////////////////////////////////////////
//close the finger function
bool FingerCommon::CloseFinger(char* pPrinterName)
{
    LOGLOG("\r\n####FM: CloseFinger===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);

    return cmd.SetFingerStatus(0);
}
//////////////////////////////////////////////////////////////////////
//check whether the user finger is exist on the device
bool FingerCommon::CheckFinger(char* pPrinterName, char* userName)
{
    LOGLOG("\r\n####FM: CheckFinger===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);
//	char name[33];

//	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,userName,-1,NULL,0,NULL,FALSE);

//	WideCharToMultiByte(CP_OEMCP,NULL,userName,-1,name,dwNum,NULL,FALSE);

//	return cmd.Check(name, dwNum);
    return cmd.Check(userName, strlen(userName));

}
////////////////////////////////////////////////////////////////////////////////////
//cancel current printing
bool FingerCommon::CancelFingerPrint(char* pPrinterName)
{
    LOGLOG("\r\n####FM: CancelFingerPrint===Enter");
#if OPEN_ONE
    if(!cmd.m_bFingerPrint)
    {

        cmd.m_bIsFingerPrintCancel = true;

        return true;
    }

    else
#endif
    {
        //FingCmd cmd(pPrinterName);
        //cmd.SetFingCmdPrinter(pPrinterName);

        //return cmd.CancelPrint();
        cmd.m_bIsFingerPrintCancel = true;
        //if(!cmd.m_bFingerPrint)
        {
            cmd.SetFingCmdPrinter(pPrinterName);
            return cmd.CancelPrint();
        }
        //return true;
    }
}

///////////////////////////////////////////////////////////////////////////////////
//delete current user finger from device
int FingerCommon::DeleteFinger(char* pPrinterName, char* userName)
{
    LOGLOG("\r\n####FM: DeleteFinger===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);
//	char name[33];

//	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,userName,-1,NULL,0,NULL,FALSE);

//	WideCharToMultiByte(CP_OEMCP,NULL,userName,-1,name,dwNum,NULL,FALSE);

//	return cmd.Delete(0, name, dwNum);
    return cmd.Delete(0, userName, qstrlen(userName));

}
///////////////////////////////////////////////////////////////////////
//delete all user fingers from device
int FingerCommon::DeleteFingers(char* pPrinterName)
{
    LOGLOG("\r\n####FM: DeleteFingers===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);

    return cmd.Delete(1,NULL, 0);

}

///////////////////////////////////////////////////////////////////////////////////////////////
//check finger
bool FingerCommon::IsFingerPrint(char* pPrinterName, char* userName, short* pIndex)
{
   LOGLOG("\r\n####FM: IsFingerPrint===Enter");
   LOGLOG("\r\n####FM: IsFingerPrint: %s", pPrinterName);

   //FingCmd cmd(pPrinterName);
   cmd.SetFingCmdPrinter(pPrinterName, true);
   int nResult = cmd.IsPrint(userName, pIndex);
   cmd.m_bFingerPrint = false;
   if(cmd.m_bIsFingerPrintCancel)
   {
       cmd.m_bIsFingerPrintCancel = false;
   }
   return nResult == _ACK? true: false;
}


#if 0
///////////////////////////////////////////////////////////////////////////////////////////////
//check finger

DWORD WINAPI WaitThread(FG_PRINT_DATA_T *ppdata)
{
    DebugMsg("\r\n####FM: WaitThread===Enter");

    CFingCmd cmd(ppdata->printerName, TRUE);

    int nTry = 3;

    int nRet = _CMD_invalid;

    while(nTry>0)
    {
        DebugMsg("try is %d", nTry);
        int nRet = cmd.IsPrint(ppdata->userName, &ppdata->ID);
        if(nRet == _ACK)
        {
            if(g_bClose == FALSE && g_hPrintDlg != NULL)
            {
                SendMessage(g_hPrintDlg, WM_DESTROY, 0, 0);
            }
            ppdata->nResult = _ACK;
            return 1;
        }
        else
        {
            DebugMsg("fail, retry");
        }
        //Sleep(30000);
        nTry--;
    }
    ppdata->nResult = nRet;
    return 0;
}

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
    DebugMsg("\r\n####FM: TimerProc===Enter");

    static int g_nTime = 30;
    if(idEvent == 1)
    {
        SetDlgItemInt(hwnd, IDC_STATIC_TIME, g_nTime, FALSE);
        g_nTime--;
        if(g_nTime <=0)
        {
            KillTimer(hwnd, 1);
            SendMessage(g_hPrintDlg, WM_DESTROY, 0, 0);
        }
    }
}
BOOL FingerWaitDlgProc
(
 HWND	hDlg,
 UINT	message,
 WPARAM	wParam,
 LPARAM	lParam
 )
{
    BOOL	fResult = FALSE;

    switch (message)
    {
    case WM_INITDIALOG:
        g_hPrintDlg = hDlg;

        SetDlgItemInt(hDlg, IDC_STATIC_TIME, 30, FALSE);

        SetTimer(hDlg, 1, 1000, TimerProc);
        break;

    case WM_COMMAND:
        {
            switch(LOWORD (wParam))
            {
            case IDCANCEL:
                EndDialog(hDlg, FALSE);
                KillTimer(hDlg, 1);
                g_bClose = TRUE;
                g_hPrintDlg = NULL;
                break;

            default :
                break;
            }
        }
        break;

    case WM_DESTROY:
        EndDialog(hDlg, TRUE);
        KillTimer(hDlg, 1);
        g_hPrintDlg = NULL;
        g_bClose = TRUE;
        break;

    default:
        break;
    }

    return fResult;
}

////////////////////////////////////////////////////////////////////////////////////////////////

COMM_API BOOL __stdcall IsFingerPrint(TCHAR* pPrinterName, TCHAR* userName, SHORT* pIndex)
{
    DebugMsg("\r\n####FM: IsFingerPrint");

    CFingCmd cmd(pPrinterName, TRUE);
    char name[33];
    SHORT index;

    //if(cmd.GetFingerStatus() == TRUE)
    {
        DWORD dwThreadId=0, dwWaitThreadID=0;
        HANDLE	hThreadWait;
        TCHAR szTmp[512];
        DLGPROC 	  lpfnDlg;
        BOOL		  fValOk = FALSE;
        DWORD exitCode = 0;
        FG_PRINT_DATA_T fgData;

        memset(&fgData, 0, sizeof(FG_PRINT_DATA_T));
        wcscpy(fgData.printerName, pPrinterName);

        hThreadWait = CreateThread(
            NULL,              // default security attributes
            0,                 // use default stack size
            (LPTHREAD_START_ROUTINE)WaitThread,        // thread function
            &fgData,             // argument to thread function
            0,                 // use default creation flags
            &dwThreadId);   // returns the thread identifier

        if (hThreadWait == NULL)
        {
            ExitProcess(0);
            DebugMsg("Create Thread Fail");
            return _Parameter_invalid;
        }

        lpfnDlg = (DLGPROC)MakeProcInstance (FingerWaitDlgProc, g_hUiModule) ;
        fValOk = (BOOL)DialogBoxParam ((HINSTANCE)g_hUiModule, MAKEINTRESOURCE(IDD_FINGER_PRINT), NULL, lpfnDlg, NULL) ;
        FreeProcInstance (lpfnDlg) ;

        if(!fValOk)
        {
            DebugMsg("\r\n####FM: User cancel printing");

            exitCode = 0;
            GetExitCodeThread(hThreadWait, &exitCode);

            if(exitCode != STILL_ACTIVE)
            {
                DebugMsg("\r\n####FM: printing has finnised");

                CloseHandle(hThreadWait);
                hThreadWait = NULL;
            }

            return _Printer_error;
        }
        else
        {
            int count=0;

            exitCode = 0;

            while(exitCode == STILL_ACTIVE)
            {
                GetExitCodeThread(hThreadWait, &exitCode);
                DebugMsg("\r\n####FM: exit code is %x", exitCode);
                Sleep(50);
            }

            DebugMsg("finished!, user name is %s, index is %d", fgData.userName, fgData.ID);
            if(strlen(fgData.userName) >0)
            {
                DWORD dwNum = MultiByteToWideChar(CP_OEMCP,NULL,fgData.userName,-1,NULL,0);

                MultiByteToWideChar(CP_OEMCP,NULL,fgData.userName,-1,userName,dwNum);

                *pIndex = fgData.ID;

                return _ACK;
            }
        }
    }

    return _Printer_Time_Out;

}
#endif

////////////////////////////////////////////////////////////////////////////////////
//add a finger for current user
int FingerCommon::AddFinger(char* pPrinterName, int mode, char* userName, int* ret_index)
{
    LOGLOG("\r\n####FM: AddFinger===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);
    char name[33];
    short index;
    int nResult = _Printer_Time_Out;

//	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,userName,-1,NULL,0,NULL,FALSE);

//	WideCharToMultiByte(CP_OEMCP,NULL,userName,-1,name,dwNum,NULL,FALSE);

    if(mode == 0)
    {
//		return  cmd.AddFingerFirst(name);
        return  cmd.AddFingerFirst(userName);
    }
    else
    {
        char name1[33] ={0};
        strcpy(name, userName);
        if(cmd.AddFingerSecond(name1, &index) == _ACK)
        {
             LOGLOG("####FM:AddFinger(): name1=%s",name1);
             LOGLOG("####FM:AddFinger(): name =%s",name);

            if(strcmp(name1, name) == 0)
            {
                *ret_index = index;
                return _ACK;
            }
        }
    }

    return nResult;

}
/////////////////////////////////////////////////////////////////////////////////
//import the finger data to device from file
bool FingerCommon::ImportFinger(char* pPrinterName, char* fileName)
{
    LOGLOG("\r\n####FM: ImportFinger===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);

    int nResult = _Printer_Time_Out;
    FG_DATAF_T* pFGData;
    char* pData = NULL;

    DWORD dwFileSize, dwRead, cbRead;

    QFile file(fileName);


    if(!file.exists()|| !file.open(QFile::ReadOnly))
    {
        return _Parameter_invalid;
    }

    dwFileSize = file.size();//GetFileSize(hFile, NULL);

    pData = (char*)malloc(dwFileSize + 1);

    if(pData != NULL)
    {
        char* pTemp = pData;

        int len = dwFileSize/sizeof(FG_DATA_T);

        while(dwFileSize>0)
        {
            if(dwFileSize < 4096)
                dwRead = dwFileSize;
            else
                dwRead = 4096;

            //ReadFile( hFile, pTemp, dwRead, &cbRead, NULL);
            cbRead = file.read(pTemp, dwRead);

            if(cbRead <= 0)//(cbRead == 0)
                break;

            dwFileSize -= cbRead;

            pTemp += cbRead;
        }
        file.close();
        //CloseHandle(hFile);

        pFGData = (FG_DATAF_T*)pData;

        nResult = cmd.Import(pFGData, len);

        free(pData);
        pData = NULL;

    }

    return nResult;

}
///////////////////////////////////////////////////////////////////////////////
//export all user finger from device to file
int FingerCommon::ExportFinger(char* pPrinterName, char* fileName, int *pdwLen)
{
    LOGLOG("\r\n####FM: ExportFinger===Enter");

    //FingCmd cmd(pPrinterName);
    cmd.SetFingCmdPrinter(pPrinterName);

    int nResult = _Printer_Time_Out;


    if(((nResult = cmd.Export(pdwLen)) == _ACK) && (*pdwLen > 0))
    {
        int len = *pdwLen;
        FG_DATAF_T* pfgData;

        pfgData = (FG_DATAF_T*)malloc(sizeof(FG_DATAF_T)*len);

        if(pfgData != NULL)
        {
            if(cmd.GetFingerData(pfgData, len)>0)
            {

                QFile file(fileName);
                if(file.exists())
                {
                    file.remove();
                }
                LOGLOG("Export file name is %s", fileName);


                if(!file.open(QFile::ReadWrite))
                {
                    if(pfgData != NULL)
                    {
                        free(pfgData);
                        pfgData = NULL;
                    }
                    return _Parameter_invalid;
                }

                DWORD dwSize = sizeof(FG_DATAF_T)*len;
                DWORD dwWritten, cbWritten;
                char* pData = (char*)pfgData;

                while(dwSize > 0)
                {
                    if(dwSize < 4096)
                        dwWritten = dwSize;
                    else
                        dwWritten = 4096;

                    //WriteFile(hFile, pData, dwWritten, &cbWritten, NULL);
                    cbWritten=file.write(pData, dwWritten);

                    if(cbWritten < 0)
                        break;

                    pData += cbWritten;
                    dwSize -= cbWritten;
                }
                file.close();
                //CloseHandle(hFile);
                if(pfgData != NULL)
                {
                    free(pfgData);
                    pfgData = NULL;
                }
                return _ACK;
            }
        }

        free(pfgData);
        pfgData = NULL;
    }
    return nResult;

}
