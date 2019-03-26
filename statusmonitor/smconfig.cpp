#include "smconfig.h"
const char* status_file = "/tmp/.statusmonitor";
const char* statusKey = "printer/status/";
const char* printersKey = "printer/mdl/";
const char* status_lock_file = "/tmp/.lockstatusmonitor";
const char* g_status_server_path = SERVER_PATH;//"/tmp/gssp.domain";//filterstatusthread use
static bool _isDeviceSupported(Printer_struct* ps)
{
    LOGLOG("found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
    return true;
}

func_isDeviceSupported isDeviceSupported = _isDeviceSupported;

SMConfig::SMConfig()
{

}
