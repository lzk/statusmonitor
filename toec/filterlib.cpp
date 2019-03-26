#include "fingermanager.h"
#include "jkinterface.h"
#include <QString>
#include "toecconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

int checkFinger(int jobid)
{
    FingerManager fm;
    return fm.checkFinger(SERVER_PATH ,jobid);
}

int get_device_id_via_filter(const char* printer_name ,const char* printer_uri)
{

    if(!QString(printer_uri).startsWith("usb://")){
        LOGLOG("filter_status_task: only usb uri can do!");
        return -1;
    }

    int result;
    char data[2049];
    int datalen = 2048;
    result = cups_usb_getDeviceID(data ,datalen);
    LOGLOG("filter_status_task get device id: %s" ,data);

    Trans_Client tc(SERVER_PATH);
    char buffer[2148];
    sprintf(buffer ,"dvid://%s?deviceid=%s" ,printer_name ,data);//error when data is null
    tc.writeThenRead(buffer ,2148);

    return result;
}

#ifdef __cplusplus
}
#endif
