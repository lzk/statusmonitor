#ifndef UICONFIG_H
#define UICONFIG_H

#include "statusmonitor.h"
#include <QObject>
#include <QMetaType>

#define SERVER_PATH "/tmp/.tomcat.domain"

Q_DECLARE_METATYPE(PrinterInfo_struct)
Q_DECLARE_METATYPE(QList<PrinterInfo_struct>)
#ifdef TOMCAT
Q_DECLARE_METATYPE(QList<Job_struct>)
#endif

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
