#include "fingermanager.h"
#include "jkinterface.h"
#include "finger.h"
#include "filter_check_finger.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef QT_NO_DEBUG
#define SERVER_PATH "/var/spool/cups/tmp/tjgd1zsm.domain"
#else
#define SERVER_PATH "/tmp/tjgd1zsm.domain"
#endif
const char* ui_server_path = SERVER_PATH;
FingerManager::FingerManager()
{

}

static const char* job_history_file_name = "/tmp/job_history.txt";
void callback_getJob(void* para,Job_struct* js)
{
    FingerManager* sm = (FingerManager*)para;
    if(js->id != sm->job_id)
        return;

    Trans_Client tc(sm->server_path);
    char buffer[256];
    bool record_list = false;
    sprintf(buffer ,"record://%s?jobid=%d" ,js->printer ,js->id);
    tc.writeThenRead(buffer ,sizeof(buffer));
    if(!strcmp(buffer ,"record")){
        record_list = true;
    }

    int isFingerChecked = 1;
    int isFingerEnable  = finger_isEnabled() ?1 :0;
    int printerResult = 1;
    if(isFingerEnable){
        //check finger
        sprintf(buffer ,"check://%s?jobid=%d" ,js->printer ,js->id);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(!strcmp(buffer ,"ok")){
            isFingerChecked = 1;
            printerResult = 1;
            sm->check_result = Checked_Result_OK;
        }else if(!strcmp(buffer ,"cancel")){
            isFingerChecked = 0;
            printerResult = 0;
            sm->check_result = Checked_Result_Cancel;
        }else{
            isFingerChecked = 0;
            printerResult = 0;
            sm->check_result = Checked_Result_Fail;
        }
        LOGLOG("check result:%s" ,buffer);
    }else{
        sm->check_result = Checked_Result_Disable;
    }

    if(record_list){
        char hostname[256];
        gethostname(hostname ,sizeof(hostname));
    //    StatusMonitor* sm = (StatusMonitor*)para;
        char job_history[256];
        sprintf(job_history ,"echo \"%d,%s,%s,%s,%s,%d,%d,%ld,%d,%d,%d\" >> %s" ,js->id ,js->printer ,hostname,js->user_name  ,js->name
                ,js->pages_completed ,js->copies ,js->timet
                ,isFingerEnable,isFingerChecked,printerResult//是，成功，否
                ,job_history_file_name);
        system(job_history);
    }
}

int FingerManager::checkFinger(const char* server_path ,int jobid)
{
    LOGLOG("libtoec: start check finger job id:%d" ,job_id);
    this->job_id = jobid;
    this->server_path = server_path;
    check_result = Checked_Result_invalidJobid;
    cups_get_job(callback_getJob ,this);
    return check_result;
}

int FingerManager::getJobHistory(CALLBACK_getJobHistory callback,void* para)
{
    char str[1024];
    int length;
    FILE* stream = fopen(job_history_file_name ,"r");
    if(!stream){
        LOGLOG("can not open job history file");
        return -1;
        }
    fseek(stream,0,SEEK_SET);
    while(!feof(stream)){
        memset(str ,0 ,sizeof(str));
        if(NULL != fgets(str,size_t(str),stream)){
            //replace fgets last \n to 0
            LOGLOG("job history:%s" ,str);
            length = strlen(str);
            str[length - 1] = 0;
            callback(para ,str);
        }else{
            LOGLOG("can not get job history");
            break;
        }
    }
    fclose(stream);
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
int checkFinger(int jobid)
{
    FingerManager fm;
    return fm.checkFinger(ui_server_path ,jobid);
}

#ifdef __cplusplus
}
#endif
