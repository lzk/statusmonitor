#ifndef UICONFIG_H
#define UICONFIG_H

#include "statusmonitor.h"
#include <QObject>
#include <QMetaType>
#ifdef TOMCAT
#define SERVER_PATH "/tmp/tomcat.domain"
#else
#define SERVER_PATH "/tmp/common.domain"
#endif

Q_DECLARE_METATYPE(PrinterInfo_struct)
Q_DECLARE_METATYPE(QList<PrinterInfo_struct>)
#ifdef TOMCAT
Q_DECLARE_METATYPE(QList<Job_struct>)
#endif
class UIConfig : public QObject {
    Q_OBJECT
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
