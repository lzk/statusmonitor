#include "StatusMonitor.h"
#include "usbio.h"
#include "log.h"
#include "finger.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "trans.h"
static int _base64_char_value(char base64char)
 {
    if (base64char >= 'A' && base64char <= 'Z')
         return base64char-'A';
    if (base64char >= 'a' && base64char <= 'z')
         return base64char-'a'+26;
    if (base64char >= '0' && base64char <= '9')
         return base64char-'0'+2*26;
    if (base64char == '+')
         return 2*26+10;
    if (base64char == '/')
         return 2*26+11;
    return -1;
}

static int _base64_decode_triple(char quadruple[4], unsigned char *result)
 {
    int i, triple_value, bytes_to_decode = 3, only_equals_yet = 1;
    int char_value[4];

    for (i=0; i<4; i++)
         char_value[i] = _base64_char_value(quadruple[i]);

    for (i=3; i>=0; i--)
    {
         if (char_value[i]<0)
         {
             if (only_equals_yet && quadruple[i]=='=')
             {
                  char_value[i]=0;
                  bytes_to_decode--;
                  continue;
             }
             return 0;
         }
         only_equals_yet = 0;
    }

    if (bytes_to_decode < 0)
         bytes_to_decode = 0;

    triple_value = char_value[0];
    triple_value *= 64;
    triple_value += char_value[1];
    triple_value *= 64;
    triple_value += char_value[2];
    triple_value *= 64;
    triple_value += char_value[3];

    for (i=bytes_to_decode; i<3; i++)
         triple_value /= 256;
    for (i=bytes_to_decode-1; i>=0; i--)
    {
         result[i] = triple_value%256;
         triple_value /= 256;
    }

    return bytes_to_decode;
}

static size_t Base64Decode(char *source, unsigned char *target, size_t targetlen)
 {
    char *src, *tmpptr;
    char quadruple[4], tmpresult[3];
    int i; size_t tmplen = 3;
    size_t converted = 0;

    src = (char *)malloc(strlen(source)+5);
    if (src == NULL)
         return -1;
    strcpy(src, source);
    strcat(src, "====");
    tmpptr = src;
    while (tmplen == 3)
    {
         /* get 4 characters to convert */
         for (i=0; i<4; i++)
         {
             while (*tmpptr != '=' && _base64_char_value(*tmpptr)<0)
                  tmpptr++;
             quadruple[i] = *(tmpptr++);
         }
         tmplen = _base64_decode_triple(quadruple, (unsigned char*)tmpresult);
         if (targetlen < tmplen)
         {
             free(src);
             return converted;
         }
         memcpy(target, tmpresult, tmplen);
         target += tmplen;
         targetlen -= tmplen;
         converted += tmplen;
    }
    free(src);
    return converted;
}


#pragma pack(1)

typedef signed char INT8;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct
{
	////////////////////////////////////////////////////
	// Consumable 
	////////////////////////////////////////////////////
	BYTE	TonelStatusLevelK;
	BYTE	TonelStatusLevelC;
	BYTE	TonelStatusLevelM;
	BYTE	TonelStatusLevelY;
	BYTE	DrumStatusLifeRemain;

	////////////////////////////////////////////////////
	// Covers 
	////////////////////////////////////////////////////
	BYTE	CoverStatusFlags;

	////////////////////////////////////////////////////
	// Paper Tray
	////////////////////////////////////////////////////
	BYTE	PaperTrayStatus;
	BYTE	PaperSize;

	////////////////////////////////////////////////////
	// Output Tray
	////////////////////////////////////////////////////
	BYTE	OutputTrayLevel;

	////////////////////////////////////////////////////
	// General Status and information
	////////////////////////////////////////////////////
	BYTE	PrinterStatus;
//	wchar_t	OwnerName[16];
//	wchar_t	DocuName[16];
    char	OwnerName[32];
    char	DocuName[32];

	BYTE	ErrorCodeGroup;
	BYTE	ErrorCodeID;
	WORD	PrintingPage;
	WORD	Copies;
	DWORD	TotalCounter;
	BYTE	TRCCurve[12];

	BYTE	TonerSize[4];

	BYTE	PaperType;
	BYTE	NonDellTonerMode;
	BYTE	AioStatus;
	//BYTE	bReserved;
	BYTE	bPhyPrinterStatus;
	//WORD	wReserved1;
	BYTE	LowTonerAlert;
	BYTE	bReserved1;
	WORD	wReserved2;
} PRINTER_STATUS;
#pragma pack()

int DecodeStatusFromDeviceID(char* device_id, PRINTER_STATUS* status)
{
    if (device_id==NULL || status==NULL) {
        return -1;
    }
    char *p = device_id;

    while (*p && strncmp(p,"STS:",4)!=0) // Look for "STS:"
        p++;

    if (!*p)	{ // "STS:" not found
        LOGLOG("STS: not found");
        return -1;
    }
    p += 4;	// Skip "STS:"

    if (Base64Decode(p, (unsigned char*)status, sizeof(PRINTER_STATUS)) != sizeof(PRINTER_STATUS))
    {
//        return -1;
    }
    return 0;
}

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

int StatusMonitor::getPrinterStatus(const char* printer_name ,PrinterStatus_struct* ps)
{
    int ret = 0;
    PRINTER_STATUS ps_static;
    char buffer[1024];
    char device_uri[256];
    cups_get_device_uri(printer_name ,device_uri);
    int dclass = getPrinterClass(device_uri);
    switch (dclass) {
    case DCLASS_usb:
    {
        UsbIO io;
        ret = io.open(0x0550 ,0x0175 ,NULL);
        if(!ret){
            ret = io.getDeviceId(buffer ,sizeof(buffer));
            io.close();
        }
    }
        break;
    case DCLASS_network:
    {
        ret = snmpGetDeviceID(device_uri ,buffer ,sizeof(buffer));
    }
        break;
    default:
        ret = -2;//unknown device class
        break;
    }
    if(!ret){
        LOGLOG("device id:%s" ,buffer);
        ret = DecodeStatusFromDeviceID(buffer ,&ps_static);
        if(!ret){
            ret = parsePrinterStatus(&ps_static ,ps);
        }
    }
    return ret;
}

struct PrinterList_para_struct
{
    CALLBACK_getPrinterInfo callback;
    void* para;
};

void getPrinterList(void* para ,Printer_struct* ps)
{
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

int StatusMonitor::getPrinters(CALLBACK_getPrinterInfo callback,void* para)
{
    struct PrinterList_para_struct pl_para;
    pl_para.callback = callback;
    pl_para.para = para;
    return cups_get_printers(getPrinterList ,(void*)&pl_para);
//    return cups_get_printers(callback ,para);
}

int StatusMonitor::getJobs(CALLBACK_getJob callback,void* para,const char* printer ,const char* which ,const char* users)
{
//    return cups_get_printers(getPrinterList ,(void*)this);
    return cups_get_job(callback ,para ,printer ,which ,users);
}

int StatusMonitor::getPrinterClass(const char* device_uri)
{
    int dclass = DCLASS_unknown;
    if(!strncmp(device_uri,"usb://" ,6)){
        dclass = DCLASS_usb;
    }else{
        dclass = DCLASS_network;
    }
    return dclass;
}

const char* StatusMonitor::getDefaultPrinter()
{
    return cups_get_default_printer();
}

int StatusMonitor::getJobHistory(CALLBACK_getJobHistory callback,void* para)
{
    FingerManager fm;
    return fm.getJobHistory(callback ,para);
}

int StatusMonitor::resolve_uri(const char* device_uri ,char* buffer ,int bufsize)
{
    return cups_resolve_uri(device_uri ,buffer ,bufsize);
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
