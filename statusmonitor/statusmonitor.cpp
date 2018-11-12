#include "statusmonitor.h"
#include "log.h"
#include "statusmanager.h"
#include "status.h"
#include "smconfig.h"

int parsePrinterStatus(PRINTER_STATUS* pStatus ,PrinterStatus_struct* ps)
{
    ps->PrinterStatus = pStatus->PrinterStatus;
    LOGLOG("get device status correct:%#.02x" ,pStatus->PrinterStatus);

    if(pStatus->TonelStatusLevelC & 0x80){
        LOGLOG("Tonel not installed");
        ps->TonelStatusLevelC = -1;
    }else{
        LOGLOG("Tonel has been installed,TonelStatusLevelC level:%d" ,pStatus->TonelStatusLevelC);
        ps->TonelStatusLevelC = pStatus->TonelStatusLevelC;
    }

    if(pStatus->TonelStatusLevelY & 0x80){
        LOGLOG("Tonel not installed");
        ps->TonelStatusLevelY = -1;
    }else{
        LOGLOG("Tonel has been installed,TonelStatusLevelY level:%d" ,pStatus->TonelStatusLevelY);
        ps->TonelStatusLevelY = pStatus->TonelStatusLevelY;
    }

    if(pStatus->TonelStatusLevelM & 0x80){
        LOGLOG("Tonel not installed");
        ps->TonelStatusLevelM = -1;
    }else{
        LOGLOG("Tonel has been installed,TonelStatusLevelM level:%d" ,pStatus->TonelStatusLevelM);
        ps->TonelStatusLevelM = pStatus->TonelStatusLevelM;
    }

    if(pStatus->TonelStatusLevelK & 0x80){
        LOGLOG("Tonel not installed");
        ps->TonelStatusLevelK = -1;
    }else{
        LOGLOG("Tonel has been installed,TonelStatusLevelK level:%d" ,pStatus->TonelStatusLevelK);
        ps->TonelStatusLevelK = pStatus->TonelStatusLevelK;
    }
    switch(pStatus->PaperTrayStatus){
    case 0xff:
        LOGLOG("paper tray status: tray no installed");           ps->trayPaperTrayStatus = 1 ;break;
    case 0xfe:
        LOGLOG("paper tray status: out of page");                 ps->trayPaperTrayStatus = 2 ;break;
    case 0:
    default:
        LOGLOG("paper tray status: ready");                       ps->trayPaperTrayStatus = 0 ;break;
    }
//    switch(pStatus->job){
//    case 0:            LOGLOG("executing job: unknow job");            break;
//    case 1:            LOGLOG("executing job: print job");            break;
//    case 2:            LOGLOG("executing job: normal copy job");            break;
//    case 3:            LOGLOG("executing job: scan job");            break;
//    case 4:            LOGLOG("executing job: fax job");            break;
//    case 5:            LOGLOG("executing job: ifax job");            break;
//    case 6:            LOGLOG("executing job: report job");            break;
//    case 7:            LOGLOG("executing job: n in 1 copy job");            break;
//    case 8:            LOGLOG("executing job: id card copy job");            break;
//    case 9:            LOGLOG("executing job: id card copy mode");            break;
//    default:            LOGLOG("executing job: print job");            break;
//    }

//            QString owner_name;
//            owner_name.setRawData((const QChar*)pStatus->OwnerName ,16);
//            LOGLOG("owner name:" + owner_name);
//            QString docu_name;
//            docu_name.setRawData((const QChar*)pStatus->DocuName ,16);
//            LOGLOG("docu name:" + docu_name);
    ps->ErrorCodeGroup = pStatus->ErrorCodeGroup;
    ps->ErrorCodeID = pStatus->ErrorCodeID;
    ps->PaperSize = pStatus->PaperSize;
    ps->PaperType = pStatus->PaperType;
    ps->LowTonerAlert = pStatus->LowTonerAlert;
    ps->NonDellTonerMode = pStatus->NonDellTonerMode;
    return 0;
}

StatusMonitor::StatusMonitor()
{
}

int StatusMonitor::getPrinterStatus(const char* printer ,PrinterStatus_struct* ps)
{
    PRINTER_STATUS ps_static;
    StatusManager statusmanager;
    int ret = statusmanager.getStatusFromFile(printer ,&ps_static);
    if(!ret){
        ret = parsePrinterStatus(&ps_static ,ps);
    }
    return ret;
}

int StatusMonitor::getDeviceStatus(DeviceIO* device ,PrinterStatus_struct* ps)
{
    PRINTER_STATUS ps_static;
    int ret = getStatusFromDevice(device ,&ps_static);
    if(!ret){
        ret = parsePrinterStatus(&ps_static ,ps);
    }
    return ret;
}

struct PrinterList_para_struct
{
    CALLBACK_getPrinterInfo callback;
    void* para;
};

static void getPrinterList(void* para ,Printer_struct* ps)
{
    if(isDeviceSupported && isDeviceSupported(ps)){

        struct PrinterList_para_struct* pl_para = (struct PrinterList_para_struct*) para;

        struct PrinterInfo_struct pis;
        pis.printer = *ps;

    //    if(!StatusMonitor::getPrinterStatus(ps->name ,&pis.status)){
    //        LOGLOG("can not get printer status");
    //    }

        if(pl_para->callback){
            pl_para->callback(pl_para->para ,&pis);
        }
    }
}

int StatusMonitor::getPrinters(CALLBACK_getPrinterInfo callback,void* para)
{
    struct PrinterList_para_struct pl_para;
    pl_para.callback = callback;
    pl_para.para = para;
    return cups_get_printers(getPrinterList ,(void*)&pl_para);
//    return cups_get_printers(callback ,para);
}

bool StatusMonitor::AnyTonerReachLevel1(const PrinterStatus_struct& m_PrinterStatus)
{
    if (m_PrinterStatus.PrinterStatus == PS_ERROR_POWER_OFF)
        return false;
    if (m_PrinterStatus.TonelStatusLevelC <= 20 || m_PrinterStatus.TonelStatusLevelM <= 20 || m_PrinterStatus.TonelStatusLevelY <= 20 || m_PrinterStatus.TonelStatusLevelK <= 20)
        return true;
    else	return false;
}

bool StatusMonitor::IsNonDellTonerMode(const PrinterStatus_struct& m_PrinterStatus)
{
    if (m_PrinterStatus.NonDellTonerMode)
        return true;
    else	return false;
}

bool StatusMonitor::OnlyColorTonerEmpty(const PrinterStatus_struct& m_PrinterStatus)
{
    if (m_PrinterStatus.PrinterStatus == PS_ERROR_POWER_OFF)
        return false;
    if (m_PrinterStatus.TonelStatusLevelK != 0 && (m_PrinterStatus.TonelStatusLevelC == 0 || m_PrinterStatus.TonelStatusLevelM == 0 || m_PrinterStatus.TonelStatusLevelY == 0))
        return true;
    else	return false;
}
