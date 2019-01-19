#include "smconfig.h"
const char* status_filename = "/tmp/.statusmonitor";
const char* statusKey = "printer/status/";
const char* printersKey = "printer/mdl/";
const char* lockfile = "/tmp/.lockstatusmonitor";

static bool _isDeviceSupported(Printer_struct* ps)
{
    LOGLOG("found device name:%s \n\tmodel:%s" ,ps->name,ps->makeAndModel);
    return true;
}

func_isDeviceSupported isDeviceSupported = _isDeviceSupported;

SMConfig::SMConfig()
{

}
