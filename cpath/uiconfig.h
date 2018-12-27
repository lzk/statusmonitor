#ifndef UICONFIG_H
#define UICONFIG_H

#include "statusmonitor.h"
#include <QObject>
#include <QMetaType>
#include "lshellstruct.h"

#define SERVER_PATH "/tmp/test.domain"

Q_DECLARE_METATYPE(PrinterInfo_struct)
Q_DECLARE_METATYPE(QList<PrinterInfo_struct>)
Q_DECLARE_METATYPE(copycmdset)
Q_DECLARE_METATYPE(cmdst_wifi_get)
Q_DECLARE_METATYPE(cmdst_aplist_get)
Q_DECLARE_METATYPE(cmdst_passwd)
Q_DECLARE_METATYPE(net_info_st)
Q_DECLARE_METATYPE(net_ipv6_st)
Q_DECLARE_METATYPE(cmdst_userconfig)
Q_DECLARE_METATYPE(cmdst_softap)

//Q_DECLARE_METATYPE(cmdst_wifi_status)
//Q_DECLARE_METATYPE(cmdst_tonerEnd)
//Q_DECLARE_METATYPE(cmdst_PSave_time)
//Q_DECLARE_METATYPE(cmdst_powerOff_time)
//Q_DECLARE_METATYPE(cmdst_fusingScReset)

typedef struct struct_wifi_refresh_info
{
    struct cmdst_wifi_get wifi_para;
    struct cmdst_aplist_get wifi_aplist;
}
    struct_wifi_refresh_info;
Q_DECLARE_METATYPE(struct_wifi_refresh_info)

#include "smconfig.h"
class UIConfig : public QObject ,SMConfig{
    Q_OBJECT
public:
    explicit UIConfig(QObject *parent = 0);

    static void initConfig();
public:
    enum CmdType{
        CMD_GetDefaultPrinter,
        CMD_GetPrinters,
        CMD_GetStatus,

        CMD_WIFI_refresh_plus,

        LS_CMD_GetCopy,
        LS_CMD_COPY,
        LS_CMD_WIFI_apply,
        LS_CMD_WIFI_apply_noread,
        LS_CMD_WIFI_get,
        LS_CMD_WIFI_getAplist,
        LS_CMD_PASSWD_set,
        LS_CMD_PASSWD_get,
        LS_CMD_PASSWD_confirm,
        LS_CMD_WIFI_GetWifiStatus,
        LS_CMD_PRN_TonerEnd_Get,
        LS_CMD_PRN_TonerEnd_Set,
        LS_CMD_PRN_PSaveTime_Get,
        LS_CMD_PRN_PSaveTime_Set,
        LS_CMD_PRN_PowerOff_Get,
        LS_CMD_PRN_PowerOff_Set,
        LS_CMD_NET_GetV4,
        LS_CMD_NET_SetV4,
        LS_CMD_NET_GetV6,
        LS_CMD_NET_SetV6,
        LS_CMD_PRN_Set_UserConfig,
        LS_CMD_PRN_Get_UserConfig,
        LS_CMD_WIFI_Set_SoftAp,
        LS_CMD_WIFI_Get_SoftAp,
        LS_CMD_PRN_FusingScReset,
    };
    Q_ENUMS(CmdType)
};

//public enum EnumStatus: byte
enum
{
    Ready                       = 0x00,
    Printing                    = 0x01,
    PowerSaving                 = 0x02,
    WarmingUp                   = 0x03,
    PrintCanceling              = 0x04,
    Processing                  = 0x07,
    CopyScanning                = 0x60,
    CopyScanNextPage            = 0x61,
    CopyPrinting                = 0x62,
    CopyCanceling               = 0x63,
    IDCardMode                  = 0x64,
    ScanScanning                = 0x6A,
    ScanSending                 = 0x6B,
    ScanCanceling               = 0x6C,
    ScannerBusy                 = 0x6D,
    TonerEnd1                   = 0x7F,//For china maket
    TonerEnd2                   = 0x80,
    TonerNearEnd                = 0x81,
    ManualFeedRequired          = 0x85,
    InitializeJam               = 0xBC,
    NofeedJam                   = 0xBD,
    JamAtRegistStayOn           = 0xBE,
    JamAtExitNotReach           = 0xBF,
    JamAtExitStayOn             = 0xC0,
    CoverOpen                   = 0xC1,
    NoTonerCartridge            = 0xC5,
    WasteTonerFull              = 0xC6,
    PDLMemoryOver              = 0xC2,
    FWUpdate                    = 0xC7,
    OverHeat                    = 0xC8,
    PolygomotorOnTimeoutError   = 0xCD,
    PolygomotorOffTimeoutError  = 0xCE,
    PolygomotorLockSignalError  = 0xCF,
    BeamSynchronizeError        = 0xD1,
    BiasLeak                    = 0xD2,
    PlateActionError            = 0xD3,
    MainmotorError              = 0xD4,
    MainFanMotorEorror          = 0xD5,
    JoinerThermistorError       = 0xD6,
    JoinerReloadError           = 0xD7,
    HighTemperatureErrorSoft    = 0xD8,
    HighTemperatureErrorHard    = 0xD9,
    JoinerFullHeaterError       = 0xDA,
    Joiner3timesJamError        = 0xDB,
    LowVoltageJoinerReloadError = 0xDC,
    MotorThermistorError        = 0xDD,
    EEPROMCommunicationError    = 0xDE,
    CTL_PRREQ_NSignalNoCome     = 0xDF,
    SCAN_USB_Disconnect         = 0xE1,
    SCAN_NET_Disconnect         = 0xE4,
    ScanMotorError              = 0xE5,
    SCAN_DRV_CALIB_FAIL         = 0xE9,
    NetWirelessDongleCfgFail    = 0xE8,
    ScanDriverCalibrationFail   = 0xE9,
    DMAError                    = 0xEF,
    Unknown                     = 0xF0, // status added by SW
    Offline                     = 0xF1, // status added by SW
    PowerOff                    = 0xF2, // status added by SW
};

#endif // UICONFIG_H
