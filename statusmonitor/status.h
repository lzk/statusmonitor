#ifndef STATUS_H
#define STATUS_H

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

class DeviceIO;
int getStatusFromDevice(DeviceIO* device ,PRINTER_STATUS* ps);
#endif // STATUS_H
