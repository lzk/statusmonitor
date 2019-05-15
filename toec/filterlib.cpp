#include "fingermanager.h"
#include "jkinterface.h"
#include <QString>
#include "toecconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool print_cancel;
extern int job_id_cancel;
extern const char* ui_server_path;

int checkFinger(int jobid ,const char* username ,const char* filename ,int use_finger_checking)
{
    FingerManager fm;
    return fm.checkFinger(SERVER_PATH ,jobid ,username ,filename ,use_finger_checking);
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

void finger_abort(int id ,const char* username ,const char* filename , const char* printname)
{

//    FingerManager fm;
    LOGLOG("gavin: finger_abort ...");
    LOGLOG("gavin: id=%d, username=%s", id, username);
    LOGLOG("gavin: filename =%s", filename);
    LOGLOG("gavin: printname=%s", printname);
    job_id_cancel = id;
    print_cancel =true;
    Trans_Client tc(ui_server_path);
    char buffer[256];
    sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",printname ,id, 9
            ,username ,filename);
    tc.writeThenRead(buffer ,sizeof(buffer));
    if(!strcmp(buffer ,"resultok"))
    {
        LOGLOG("gavin: show Dlg...close ok");
    }
    LOGLOG("gavin: finger_abort end");


}

#ifdef __cplusplus
}
#endif
