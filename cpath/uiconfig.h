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
//Q_DECLARE_METATYPE(cmdst_wifi_status)
Q_DECLARE_METATYPE(cmdst_passwd)
//Q_DECLARE_METATYPE(cmdst_tonerEnd)
//Q_DECLARE_METATYPE(cmdst_PSave_time)
//Q_DECLARE_METATYPE(cmdst_powerOff_time)
Q_DECLARE_METATYPE(net_info_st)
Q_DECLARE_METATYPE(net_ipv6_st)
Q_DECLARE_METATYPE(cmdst_userconfig)
Q_DECLARE_METATYPE(cmdst_softap)
//Q_DECLARE_METATYPE(cmdst_fusingScReset)


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


#endif // UICONFIG_H
