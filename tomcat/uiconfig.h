#ifndef UICONFIG_H
#define UICONFIG_H

#include "statusmonitor.h"
#include "tomcat.h"
#include "toecconfig.h"
#include <QObject>
#include <QMetaType>

extern const QString app_name;

#ifndef SERVER_PATH
#ifdef LOONGSON
#define SERVER_PATH "/var/spool/cups/tmp/tjgd1zsm.domain"
#else
#define SERVER_PATH "/tmp/tjgd1zsm.domain"
#endif
#endif

Q_DECLARE_METATYPE(PrinterInfo_struct)
Q_DECLARE_METATYPE(QList<PrinterInfo_struct>)
Q_DECLARE_METATYPE(PrinterStatus_struct)

Q_DECLARE_METATYPE(Jobs_struct)

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
#ifdef TOMCAT
        CMD_GetJobs,
#endif
    };
    Q_ENUMS(CmdType)
};


#endif // UICONFIG_H