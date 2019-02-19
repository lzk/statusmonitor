#include "fingermanager.h"
#include "jkinterface.h"
#include "filter_check_finger.h"
#include "toecconfig.h"
#include <string.h>
const char* ui_server_path = SERVER_PATH;
FingerManager::FingerManager()
{

}

static const char* job_history_file_name = "/tmp/job_history.txt";
void* checkFingerThread(void *para)
{
     FingerManager* sm = (FingerManager*)para;
     LOGLOG("libtoec: start check finger job");
     //Finger mFinger;
     if(!sm->mFinger.finger_check(sm->device_uri)){
         LOGLOG("libtoec: start check finger  OK");
         sm->check_result = Checked_Result_OK;

     }else{
         LOGLOG("libtoec: start check finger  Fail");
         sm->check_result = Checked_Result_Fail;
     }
     sm->chenk_end = true;
}

void callback_getJob(void* para,Job_struct* js)
{
    FingerManager* sm = (FingerManager*)para;
    if(js->id != sm->job_id)
        return;

    Trans_Client tc(sm->server_path);
    char buffer[256];

//    bool record_list = false;
//    sprintf(buffer ,"record://%s?jobid=%d" ,js->printer ,js->id);
//    tc.writeThenRead(buffer ,sizeof(buffer));
//    if(!strcmp(buffer ,"record")){
//        record_list = true;
//    }

//    int isFingerChecked = 1;
//    int isFingerEnable  = finger_isEnabled() ?1 :0;
//    int printerResult = 1;

    //Finger mFinger;
    char device_uri[256];
    cups_get_device_uri(js->printer, device_uri);
    strcpy(m_device_uri, device_uri);
    int isFingerChecked = 1;
    int isFingerEnable  = sm->mFinger.finger_isEnabled(device_uri)? 1:0;//finger_isEnabled() ?1 :0;
    int printerResult = 1;
    sm->chenk_end = false;
    if(isFingerEnable)
    {
        LOGLOG("libtoec: start check finger job");
//        if(!mFinger.finger_check(device_uri)){
//            LOGLOG("libtoec: start check finger  OK");
//            sm->check_result = Checked_Result_OK;

//        }else{
//            LOGLOG("libtoec: start check finger  Fail");
//            sm->check_result = Checked_Result_Fail;
//        }
        int ret;
        ret = pthread_create(&check_thread, NULL, checkFingerThread, (void *)sm);
        pthread_detach(check_thread);
        if (ret != 0)
        {
            DBG(DBG_warn,"......Create thread Fail !\n");
            sm->check_result = Checked_Result_Fail;
            sm->chenk_end = true;
        }

    }
    else{
        LOGLOG("libtoec: finger  not open");
        sm->check_result = Checked_Result_Disable;
    }
#if 0
    if(1){
//    if(isFingerEnable){
        //check finger
        sprintf(buffer ,"check://%s?jobid=%d" ,js->printer ,js->id);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(!strcmp(buffer ,"ok")){
//            isFingerChecked = 1;
//            printerResult = 1;
            sm->check_result = Checked_Result_OK;
        }else if(!strcmp(buffer ,"cancel")){
//            isFingerChecked = 0;
//            printerResult = 0;
            sm->check_result = Checked_Result_Cancel;
        }else if(!strcmp(buffer ,"disable")){
//            isFingerChecked = 0;
//            printerResult = 0;
            sm->check_result = Checked_Result_Disable;
        }else if(!strcmp(buffer ,"timeout")){
//            isFingerChecked = 0;
//            printerResult = 0;
            sm->check_result = Checked_Result_timeout;
        }else{
//            isFingerChecked = 0;
//            printerResult = 0;
            sm->check_result = Checked_Result_Fail;
        }
        LOGLOG("check result:%s" ,buffer);
    }else{
        sm->check_result = Checked_Result_Disable;
    }
#endif
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
