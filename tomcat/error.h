#ifndef ERROR_H
#define ERROR_H
#include "ids_string.h"
#ifdef __cplusplus
extern "C" {
#endif
//enum{
//    //System Error
//    SYS_FAIL_SYNC_MA_WITH_LO
//    ,SYS_FAIL_COMM_WITH_IOT
//    ,SYS_MCU_AP_CODE_FAIL
//    ,SYS_CODE_UPDATE_ERROR
//    ,SYS_CODE_UPDATE_INVALID
//    //Network Error
//    ,RAW_SERV_INIT_ERROR
//    ,LPD_SERV_INIT_ERROR
//    ,SNMP_AGENT_INIT_ERROR
//    //Print Error
//    ,PRT_PJL_GDI_CMD_ERROR
//    ,PRT_PDL_MEMORY_OVERFLOW
//    ,PRT_DRV_PAPER_OUT
//    ,PRT_MGR_DECODE_ERROR
//    ,PRT_MGR_PAPERSIZE_SETTING_ERROR
//    ,PRT_MGR_PAPERTYPE_SETTING_ERROR
//    ,PRT_MGR_PAPERSIZE_MISMATCH_ERROR
//    ,RPT_SYS_PAPER_SIZE_ERROR
//    //Engine Error
//    ,PRT_DRV_K_CRU_DETACHED
//    ,PRT_DRV_C_CRU_DETACHED
//    ,PRT_DRV_M_CRU_DETACHED
//    ,PRT_DRV_Y_CRU_DETACHED
//    ,PRT_DRV_EXIT_ON_JAM
//    ,PRT_DRV_FUSER_FAIL
//    ,PRT_DRV_TONER_K_LIFE_OVER
//    ,PRT_DRV_TONER_C_LIFE_OVER
//    ,PRT_DRV_TONER_M_LIFE_OVER
//    ,PRT_DRV_TONER_Y_LIFE_OVER
//    ,PRT_DRV_REAR_COVER_OPEN
//    ,PRT_DRV_TONER_K_LIFE_WARN
//    ,PRT_DRV_TONER_C_LIFE_WARN
//    ,PRT_DRV_TONER_M_LIFE_WARN
//    ,PRT_DRV_TONER_Y_LIFE_WARN
//    ,PRT_DRV_CASSETTE_DETACHED

//}ERROR_CODE;


typedef struct Error_struct
{
    int group;
    int id;
    int string_index;
    const char* code;
    const char* showMeHowFileName;
} Error_struct;

typedef struct Error_String_struct
{
    int string_index;
    const char* title;
    int mediaInfo;
    int lines;
    const char* line0;
    const char* line1;
    const char* line2;
    const char* line3;
    const char* line4;
} Error_String_struct;

typedef struct ErrorInfo_struct
{
    const Error_struct* error;
    const Error_String_struct* errorString;
    char paperSizeString[128];
    char paperTypeString[128];
}
ErrorInfo_struct;
ErrorInfo_struct getErrorInfo(int group ,int id ,int paperType ,int paperSize);
#ifdef __cplusplus
}
#endif
#endif // ERROR_H
