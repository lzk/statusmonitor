#ifndef UICONFIG_H
#define UICONFIG_H

#include "statusmonitor.h"
#include <QObject>
#include <QMetaType>

#define SERVER_PATH "/tmp/.hornet.domain"

Q_DECLARE_METATYPE(PrinterInfo_struct)
Q_DECLARE_METATYPE(QList<PrinterInfo_struct>)


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
    };
    Q_ENUMS(CmdType)
};


#endif // UICONFIG_H
