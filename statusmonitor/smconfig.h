#ifndef SMCONFIG_H
#define SMCONFIG_H

#ifndef SERVER_PATH
#ifdef LOONGSON
#define SERVER_PATH "/var/spool/cups/tmp/tjgd1zsm.domain"
#else
#define SERVER_PATH "/tmp/tjgd1zsm.domain"
#endif
#endif
#include "jkinterface.h"
typedef bool (*func_isDeviceSupported)(Printer_struct*);

extern const char* status_file;
extern const char* statusKey;
extern const char* printersKey;
extern const char* status_lock_file;
extern const char* g_status_server_path;
extern func_isDeviceSupported isDeviceSupported;
class SMConfig : public JKConfig
{
public:
    SMConfig();

};

#endif // SMCONFIG_H
