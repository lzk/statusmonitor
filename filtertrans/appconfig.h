#ifndef APCONFIG_H
#define APCONFIG_H

#ifndef SERVER_PATH
#ifdef LOONGSON
//#define SERVER_PATH "/var/run/cups/tjgd1zsm.domain"
#define SERVER_PATH "/var/spool/cups/tmp/tjgd1zsm.domain"
#else
#define SERVER_PATH "/tmp/tjgd1zsm.domain"
#endif
#endif


#define SERVER_PATH_STM "/tmp/tjgd1zsmui.domain"

#include "smconfig.h"
#include <QObject>
class AppConfig : public QObject ,SMConfig{
    Q_OBJECT
public:
    explicit AppConfig(QObject *parent = 0);

    static int initConfig();
    static void exit_app();
};
#endif // APCONFIG_H
