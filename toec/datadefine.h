#ifndef _DATADEFINE_h_
#define _DATADEFINE_h_

#pragma pack(1)
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned short WORD;


typedef struct _FG_CMD_STRCUT{
	unsigned int code;
	unsigned int data;
}FG_CMD_T;

typedef struct _FG_FUNC_STRUCT {
	unsigned int code; //"FING"
	unsigned char mode; //r: read, W:write
	unsigned short reserved;
	unsigned char func; //0: off, 1: open
} FG_FUNC_T;


typedef struct _FG_CHECK_STRUCT {
	unsigned int code; //"CHCK"
	unsigned short length; //the length of data
	unsigned short reserved;
} FG_CHECK_T;


typedef struct _FG_DELETE_STRUCT {
	unsigned int code; //"DELE"
	unsigned short length; //the length of data
	unsigned char reserved;
	unsigned char mode;  //0: delete one user, 1: delete all users
} FG_DELETE_T;


typedef struct _FG_STA_STRUCT {
	unsigned int code; //"ACK"
	unsigned char ack; //A: OK, E:error B: busy N: device not ready
	unsigned short length;
	unsigned char status; 
} FG_STA_T;


typedef struct _FG_CANCEL_STRUCT {
	unsigned int code; //"CANL"
	unsigned int reserved; //the length of data
} FG_CANCEL_T;



typedef struct _FG_DISC_STRUCT {
	unsigned int code; //"DISC"
	unsigned int reserved; //
} FG_DISC_T;



typedef struct _FG_STATUS_STRUCT {
	unsigned int code; //"STAUS"
	unsigned int reserved; //
} FG_STATUS_T;



typedef struct _FG_GETRESULT_STRUCT {
	unsigned int code; //"GETR"
	unsigned int reserved; //
} FG_GETRESULT_T;




typedef struct _FG_IMPORT_STRUCT {
	unsigned int code; //"IMPT"
	unsigned short reserved; //
	unsigned short num; //the number of user
} FG_IMPORT_T;


typedef struct _FG_EXPORT_STRUCT {
	unsigned int code; //"EXPT"
	unsigned int reserved; //
} FG_EXPORT_T;



typedef struct _FG_ADD_STRUCT {
	unsigned int code; //"ADDF"
	unsigned short length;// the length of 
	unsigned short reserved; //
} FG_ADD_T;


typedef struct _FG_DATA_STRUCT{
	unsigned char UserName[32];
	unsigned short ID;
}FG_DATA_T;

typedef struct _FG_DATAF_STRUCT{
	unsigned char UserName[32];
	unsigned short ID;
	unsigned char feature[512];
}FG_DATAF_T;

#ifndef I4
#define	I4(i) ((((i) & 0xff)<<24)+(((i) & 0xff00)<<8)+(((i) & 0xff0000)>>8)+(((i) & 0xff000000)>>24))
#endif
#ifndef I3
#define I3(i) ((((i) & 0xff)<<16)+(((i) & 0xff00))+(((i) & 0xff0000)>>16))
#endif
#ifndef I2
#define	I2(i) ((((i) & 0xff)<<8)+(((i) & 0xff00)>>8))
#endif
#ifndef I1
#define	I1(i) ((i) & 0xff)
#endif

#pragma pack()

#endif


