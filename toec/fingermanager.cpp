#include "fingermanager.h"
#include "jkinterface.h"
#include "finger.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
const char* ui_server_path = "/tmp/.tomcat.domain";
FingerManager::FingerManager()
{

}

static const char* job_history_file_name = "/tmp/job_history.txt";
void callback_getJob(void* para,Job_struct* js)
{
    FingerManager* sm = (FingerManager*)para;
    if(js->id != sm->job_id)
        return;
    char hostname[256];
    gethostname(hostname ,sizeof(hostname));
//    StatusMonitor* sm = (StatusMonitor*)para;
    char job_history[256];
    sprintf(job_history ,"echo \"%d,%s,%s,%s,%s,%d,%d,%ld,%d,%d,%d\" >> %s" ,js->id ,js->printer ,hostname,js->user_name  ,js->name
            ,js->pages_completed ,js->copies ,js->timet
            ,sm->isFingerEnable,sm->isFingerChecked,sm->printerResult//是，成功，否
            ,job_history_file_name);
    system(job_history);
}

int FingerManager::checkFinger(const char* server_path ,int jobid)
{
    int ret = 0;
    this->job_id = jobid;
    isFingerChecked = 1;
    isFingerEnable  = finger_isEnabled() ?1 :0;
    printerResult = 1;
    if(isFingerEnable){
        //check finger
        Trans_Client tc(server_path);
        char buffer[256];
        strcpy(buffer ,"check");
        ret = tc.writeThenRead(buffer ,sizeof(buffer));
        if(strcmp(buffer ,"ok")){
            ret = -1;
            isFingerChecked = 0;
            printerResult = 0;
        }
        LOGLOG("check result:%s" ,buffer);
    }
    cups_get_job(callback_getJob ,this);
    return ret;
}

int FingerManager::getJobHistory(CALLBACK_getJobHistory callback,void* para)
{
    char str[1024];
    int length;
    FILE* stream = fopen(job_history_file_name ,"r");
    if(!stream)
        return -1;
    fseek(stream,0,SEEK_SET);
    while(!feof(stream)){
        memset(str ,0 ,sizeof(str));
        if(NULL != fgets(str,size_t(str),stream)){
            //replace fgets last \n to 0
            length = strlen(str);
            str[length - 1] = 0;
            callback(para ,str);
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
