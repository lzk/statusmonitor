// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the COMM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// COMM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
/*#ifdef COMM_EXPORTS
#define COMM_API __declspec(dllexport)
#else
#define COMM_API __declspec(dllimport)
#endif


#pragma once*/

// PRINTER STATUS CODE
#define PSTATUS_Ready				0x00
#define PSTATUS_Printing                        0x01
#define PSTATUS_PowerSaving                     0x02
#define PSTATUS_WarmingUp                       0x03
#define PSTATUS_PendingDeletion                 0x04
#define PSTATUS_Paused                          0x05
#define PSTATUS_Waiting                         0x06
#define PSTATUS_Processing                      0x07
#define PSTATUS_Busy                            0x08
#define PSTATUS_Offline                         0x80
#define PSTATUS_TonerLow                        0x81
#define PSTATUS_Initializing                    0x82
#define PSTATUS_Unknown                         0x83
#define PSTATUS_Active                          0x84
#define PSTATUS_ManualFeedRequired              0x85
#define PSTATUS_PaperJam                        0xC0
#define PSTATUS_DoorOpen                        0xC1
#define PSTATUS_OutOfMemory                     0xC2
#define PSTATUS_OutOfPaper                      0xC3
#define PSTATUS_PaperProblem                    0xC4
#define PSTATUS_NoToner                         0xC5
#define PSTATUS_PageError                       0xC6
#define PSTATUS_UpdatingFW       		0xC7
#define PSTATUS_OutputBinFull                   0xC8
#define PSTATUS_NotSupport                      0xC9
#define PSTATUS_UserInterventionRequired        0xCA
#define PSTATUS_PowerOff                        0xCE
#define PSTATUS_Error                           0xCF



#define PS_READY                                  0x00
#define PS_PRINTING                               0x01
#define PS_POWER_SAVING                           0x02
#define PS_WARMING_UP                             0x03
#define PS_PENDING_DELETION                       0x04
#define PS_PAUSED                                 0x05
#define PS_WAITING                                0x06
#define PS_PROCESSING                             0x07
#define PS_BUSY                                   0x08
#define PS_OFFLINE                                0x80
#define PS_TONER_LOW                              0x81
#define PS_INITIALIZING                           0x82
#define PS_UNKNOWN                                0x83
#define PS_ACTIVE                                 0x84
#define PS_MANUAL_FEED_REQUIRED                   0x85
#define PS_ERROR_PAPER_JAM                        0xC0
#define PS_ERROR_DOOR_OPEN                        0xC1
#define PS_ERROR_OUT_OF_MEMORY                    0xC2
#define PS_ERROR_OUT_OF_PAPER                     0xC3
#define PS_ERROR_PAPER_PROBLEM                    0xC4
#define PS_ERROR_NO_TONER                         0xC5
#define PS_ERROR_PAGE_ERROR					      0xC6
#define PS_ERROR_NOT_AVAILABLE                    0xC7
#define PS_ERROR_OUTPUT_BIN_FULL                  0xC8
#define PS_ERROR_NOT_SUPPORT                      0xC9
#define PS_ERROR_USER_INTERVENTION_REQUIRED       0xCA
#define PS_ERROR_ADF_COVER_OPEN                   0xCB
#define PS_ERROR_ADF_PAPER_JAM                    0xCC
#define PS_ERROR_POWER_OFF                        0xCE
#define PS_ERROR_ERROR                            0xCF


#define MAX_SIZE_BUFF  1024//48*1000

#define  _ACK                          0
#define  _CMD_invalid                  1
#define  _Parameter_invalid            2
#define  _Do_not_support_this_function 3
#define  _Printer_busy                 4
#define  _Printer_error                5
#define  _Set_parameter_error          6
#define  _Get_parameter_error          7
#define  _Printer_is_Sleeping          8
#define  _Printer_Time_Out				9
#define  _Printer_Not_Ready			10
#define  _Printer_Finger_Wrong          11
#define  _Printer_Cancel				12
#define  _Finger_Error             13
#define  _User_Not_Exist             14


#define  _SW_USB_OPEN_FAIL                15
#define  _SW_USB_ERROR_OTHER              16
#define  _SW_USB_WRITE_TIMEOUT            17
#define  _SW_USB_READ_TIMEOUT             18
#define  _SW_USB_DATA_FORMAT_ERROR        19
#define  _SW_NET_DLL_LOAD_FAIL            21
#define  _SW_NET_DATA_FORMAT_ERROR        22
#define  _SW_UNKNOWN_PORT                 31
#define  _SW_INVALID_PARAMETER            32
#define  _SW_INVALID_RETURN_VALUE         33


#define DEVICE_NAME L"OEP3300CDN"

#define TIME_OUT 3000

#define PT_UNKNOWN 0
#define PT_TCPIP   1
#define PT_USB     2
#define PT_WSD     3
#define PT_FILE   4

#define PORT_STD_TCPIP	L"TCPMON.DLL"
#define PORT_STD_USB	L"Dynamic Print Monitor"
#define PORT_STD_WSD	L"WSD Port Monitor"

#define _PSAVE_TIME_GET			0x00
#define _PSAVE_TIME_SET			0x01


typedef struct _FG_PRINT_DATA{
	char userName[33];
	short ID;
    //TCHAR printerName[512];
    char printerName[512];
	int nResult;
}FG_PRINT_DATA_T, *LPFG_PRINT_DATA;
/*
#pragma pack(1)
typedef struct {
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
	wchar_t	OwnerName[16];
	wchar_t	DocuName[16];	
	BYTE	ErrorCodeGroup;
	BYTE	ErrorCodeID;
	WORD	PrintingPage;	
	WORD	Copies;
	DWORD	TotalCounter;	
#ifdef _GRANDE
	////////////////////////////////////////////////////
	// TRC index (For ULC only)
	////////////////////////////////////////////////////
	BYTE	TRCCurve[12];	
#else
	BYTE	reserved[12];
#endif
	BYTE	TonerCapacity[4];

	BYTE	PaperType;
#ifdef _LENOVO_
	BYTE	bReserved[2];
	BYTE	ExecutingJob;
	//BYTE	bReserved2[24];
	BYTE	bReserved2[4];
#else
	BYTE	NonDellTonerMode;
	BYTE	AioStatus;
	BYTE	bReserved;
	WORD	wReserved1;
	WORD	wReserved2;
#endif
} PRINTER_STATUS;

#pragma pack()*/



