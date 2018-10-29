#include "fingermanager.h"
#include "trans.h"
#include "api_libcups.h"
#include "log.h"
#include "finger.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
            ,1,1,0//是，成功，否
            ,job_history_file_name);
    system(job_history);
}

int FingerManager::checkFinger(int jobid)
{
    int ret;
    this->job_id = jobid;
    if(finger_isEnabled()){
        //check finger
        Trans_Client tc;
        char buffer[256];
        strcpy(buffer ,"check");
        ret = tc.writeThenRead(buffer ,sizeof(buffer));
        if(strcmp(buffer ,"ok")){
            ret = -1;
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
    return fm.checkFinger(jobid);
}

#ifdef __cplusplus
}
#endif
