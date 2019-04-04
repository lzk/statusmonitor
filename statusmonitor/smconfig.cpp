#include "smconfig.h"
const char* status_file = "/tmp/.status";
const char* status_lock_file = "/tmp/.lockstatus";
const char* statusKey = "printer/status/";
const char* printersKey = "printer/mdl/";

static bool _isDeviceSupported(Printer_struct* ps)
{
    LOGLOG("found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
    return true;
}

func_isDeviceSupported isDeviceSupported = _isDeviceSupported;

SMConfig::SMConfig()
{

}
