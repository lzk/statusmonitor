#include "fingermanager.h"
#include "jkinterface.h"
#include "filterlib.h"
#include "toecconfig.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <QString>
#include <QDir>
#include <QFile>
#include <QSettings>


bool print_cancel =false;
int job_id_cancel = -1;
const char* ui_server_path = SERVER_PATH;
FingerManager::FingerManager()
{

}

static const char* job_history_file_name = "/tmp/job_history.txt";
void* checkFingerThread(void *para)
{
     FingerManager* sm = (FingerManager*)para;
     LOGLOG("libtoec: start check finger job");
     LOGLOG("libtoec: %s" , sm->m_device_uri);
     int ret = 0;

     //Finger mFinger;
     if((ret=sm->mFinger.finger_check(sm->m_device_uri, sm->m_timeout))==0){
         LOGLOG("libtoec: start check finger  OK");
         sm->check_result = Checked_Result_OK;

     }else{
         LOGLOG("libtoec: start check finger  Fail");

         if(ret == 4)
         {
              LOGLOG("libtoec: print busy")
              sm->check_result = Checked_Result_DevBusy;
         }
         else if(ret == 9)
         {
             LOGLOG("libtoec: print timeout")
             sm->mFinger.finger_cancel(sm->m_device_uri);
             sm->check_result = Checked_Result_timeout;
         }
         else
         {
             LOGLOG("libtoec: print other error")
             sm->check_result = Checked_Result_Fail;
         }
     }
     sm->chenk_end = true;
}

void callback_getJob(void* para,Job_struct* js)
{

    FingerManager* sm = (FingerManager*)para;
    if(js->id != sm->job_id)
    {
//        LOGLOG("callback_getJob: id fail return");
        return;
    }

    LOGLOG("callback_getJob: start check finger");
    LOGLOG("callback_getJob:server_path=%s",ui_server_path);
    LOGLOG("callback_getJob user name:%s ,file name:%s" ,sm->username ,sm->filename);
    LOGLOG("callback_getJob printer:%s" ,js->printer);

    Trans_Client tc(ui_server_path);//(sm->server_path);
    char buffer[256];

    if(!sm->use_finger_checking){
        sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id,Checked_Result_noFingerModule
                ,sm->username ,sm->filename);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(!strcmp(buffer ,"resultok")){
            LOGLOG("gavin: result ok");
           // break;
        }
        return;
    }
//    bool record_list = false;
//    sprintf(buffer ,"record://%s?jobid=%d" ,js->printer ,js->id);
//    tc.writeThenRead(buffer ,sizeof(buffer));
//    if(!strcmp(buffer ,"record")){
//        record_list = true;
//    }

//    int isFingerChecked = 1;
//    int isFingerEnable  = finger_isEnabled() ?1 :0;
//    int printerResult = 1;

    pthread_t check_thread;
    //Finger mFinger;
    char device_uri[256];
    cups_get_device_uri(js->printer, device_uri);
    memset(sm->m_device_uri, 0, sizeof(sm->m_device_uri));
    strcpy(sm->m_device_uri, device_uri);
    int isFingerChecked = 1;
    LOGLOG("callback_getJob: check finge gate open ...");

    int isFingerEnable  = sm->mFinger.finger_isEnabled(device_uri);//? 1:0;//finger_isEnabled() ?1 :0;
    LOGLOG("callback_getJob: check finge gate open %d", isFingerEnable);
    if(job_id_cancel == js->id && print_cancel)
    {
        LOGLOG("gavin: print job cancel ...");
        print_cancel = false;
        job_id_cancel = -1;
        return;
    }

    int printerResult = 1;
    sm->chenk_end = false;
    bool showDlg = false;
    if(isFingerEnable == 1)
    {


        LOGLOG("gavin: show Dlg...");
        QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
        QString value = QString("%1,%2").arg(Checked_Result_checking).arg(0);
        settings.setValue(QString("%1").arg(js->id) ,value);
        settings.sync();

        int invalid_times = 0;
        sprintf(buffer ,"start://%s?jobid=%d" ,js->printer ,js->id);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(strstr(buffer ,"startok")!= NULL)
        {
            LOGLOG("gavin: show Dlg...OK");
            sscanf(buffer,"startok:%d",&(sm->m_timeout));
            LOGLOG("libtoec: start check finger job , timeout=%d", sm->m_timeout);
            if(job_id_cancel == js->id && print_cancel)
            {
                print_cancel = false;
                job_id_cancel = -1;
                LOGLOG("gavin: print job cancel 2 ...");
                sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, Checked_Result_Abort_Print
                        ,sm->username ,sm->filename);
                tc.writeThenRead(buffer ,sizeof(buffer));
                if(!strcmp(buffer ,"resultok"))
                {
                    LOGLOG("gavin: show Dlg...close ok");
                }
                return;
            }
            int ret;
            ret = pthread_create(&check_thread, NULL, checkFingerThread, (void *)sm);
            pthread_detach(check_thread);
            if (ret != 0)
            {
                LOGLOG("......Create thread Fail !\n");
                sm->check_result = Checked_Result_Fail;
                sm->chenk_end = true;
                return;

            }

            usleep(200000);
            struct timeval tpstart, tpend;/*for test time*/
            gettimeofday(&tpstart, 0);
            while(1){

                //LOGLOG("gavin: job_id_cancel = %d, js->id=%d, print_cancel=%d...",job_id_cancel, js->id, print_cancel? 1:0);
                if(job_id_cancel == js->id && print_cancel)
                {
                    print_cancel = false;
                    job_id_cancel = -1;
                    LOGLOG("gavin: print job cancel 3 ...");
                    pthread_cancel(check_thread);
                    usleep(200000);
                    sm->mFinger.finger_cancel(sm->m_device_uri);
                    sm->check_result = Checked_Result_Abort_Print;

                    sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                            ,sm->username ,sm->filename);
                    tc.writeThenRead(buffer ,sizeof(buffer));
                    if(!strcmp(buffer ,"resultok"))
                    {
                        LOGLOG("gavin: show Dlg(job cancel)...close ok");
                    }
                    break;
                }

                gettimeofday(&tpend, 0);
                float timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
                timeuse /= 1000000;
                if((timeuse -(sm->m_timeout+10)) > 0)
                {
                    pthread_cancel(check_thread);
                    usleep(200000);
                    LOGLOG("gavin: print cancel 4 ...");

                    sm->mFinger.finger_cancel(sm->m_device_uri);
                    sm->check_result = Checked_Result_timeout;
                    sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                            ,sm->username ,sm->filename);
                    tc.writeThenRead(buffer ,sizeof(buffer));
                    if(!strcmp(buffer ,"resultok"))
                    {
                        LOGLOG("gavin: show Dlg(timeout cancel)...close ok");
                    }
                    break;

                }

                sprintf(buffer ,"check://%s?jobid=%d" ,js->printer ,js->id);
                tc.writeThenRead(buffer ,sizeof(buffer));
                if(!strcmp(buffer ,"cancel"))
                {
                    pthread_cancel(check_thread);
                    usleep(200000);
                    LOGLOG("gavin: print cancel 5 ...");
                    sm->mFinger.finger_cancel(sm->m_device_uri);
                    LOGLOG("gavin: show Dlg...cancel");
    //                         while(1)
    //                         {
    //                             //wait thread finish
    //                             if(sm->chenk_end)
    //                                 break;
    //                             usleep(10000);
    //                         }
                    sm->check_result = Checked_Result_Cancel;
                    sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                            ,sm->username ,sm->filename);
                    tc.writeThenRead(buffer ,sizeof(buffer));
                    if(!strcmp(buffer ,"resultok"))
                    {
                        LOGLOG("gavin: show Dlg(cancel)...close ok");
                    }
                    break;
                }
                else if(!strcmp(buffer ,"timeout"))
                {

                    LOGLOG("gavin: show Dlg...timeout");
    //                        while(1)
    //                        {
    //                            //wait thread finish
    //                            if(sm->chenk_end)
    //                                break;
    //                            usleep(10000);
    //                        }

                    struct timeval timeout_start;
                    gettimeofday(&timeout_start, 0);
                    while(1)
                    {
                        if(sm->chenk_end)
                        {
                            LOGLOG("gavin: show Dlg...close");
                            // while(1)
                            {
                                sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                                        ,sm->username ,sm->filename);
                                tc.writeThenRead(buffer ,sizeof(buffer));
                                if(!strcmp(buffer ,"resultok"))
                                {
                                    LOGLOG("gavin: show Dlg(timeout10fix)...close ok");
                                }
                            }
                            break;
                        }
                        gettimeofday(&tpend, 0);
                        float timeuse = 1000000 * (tpend.tv_sec - timeout_start.tv_sec) + tpend.tv_usec - timeout_start.tv_usec;
                        timeuse /= 1000000;
                        if((timeuse -10) > 0)
                        {
                            break;

                        }
                        usleep(200000);

                    }
                    if(!sm->chenk_end)
                    {
                        pthread_cancel(check_thread);

                        usleep(200000);
                        LOGLOG("gavin: print cancel 6 ...");
                        sm->mFinger.finger_cancel(sm->m_device_uri);
                        sm->check_result = Checked_Result_timeout;
                        sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                                ,sm->username ,sm->filename);
                        tc.writeThenRead(buffer ,sizeof(buffer));
                        if(!strcmp(buffer ,"resultok"))
                        {
                            LOGLOG("gavin: show Dlg(timeout)...close ok");
                        }

                    }
                    break;
                }
                else if(!strcmp(buffer ,"checking"))
                {
                    invalid_times = 0;
                    //LOGLOG("gavin: show Dlg...checking");
                    usleep(200000);
                }
                else if(!strcmp(buffer ,"invalid"))
                {
                    invalid_times++;
                    LOGLOG("gavin: show Dlg...invalid");
                    //if(invalid_times >5)
                    {

                        pthread_cancel(check_thread);
                        usleep(200000);
                        LOGLOG("gavin: print cancel 7 ...");
                        sm->mFinger.finger_cancel(sm->m_device_uri);
                        sm->check_result = Checked_Result_Cancel;
                        sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                                ,sm->username ,sm->filename);
                        tc.writeThenRead(buffer ,sizeof(buffer));
                        if(!strcmp(buffer ,"resultok"))
                        {
                            LOGLOG("gavin: show Dlg...close ok");
                        }
                        break;
                    }
                    usleep(200000);
                }
                if(sm->chenk_end)
                {
                    LOGLOG("gavin: show Dlg...close");
                    // while(1)
                    {
                        sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                                ,sm->username ,sm->filename);
                        tc.writeThenRead(buffer ,sizeof(buffer));
                        if(!strcmp(buffer ,"resultok"))
                        {
                            LOGLOG("gavin: show Dlg...close ok");
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            LOGLOG("gavin: show Dlg...fail");
            //sm->check_result = Checked_Result_Fail;
            sm->m_timeout = 30;
            int ret;
            ret = pthread_create(&check_thread, NULL, checkFingerThread, (void *)sm);

            pthread_detach(check_thread);
            if (ret != 0)
            {
                LOGLOG("......Create thread Fail !\n");
                sm->check_result = Checked_Result_Fail;
                sm->chenk_end = true;
                return;

            }
            struct timeval tpstart, tpend;/*for test time*/
            gettimeofday(&tpstart, 0);

            while(1)
            {
                if(job_id_cancel == js->id && print_cancel)
                {
                    print_cancel = false;
                    job_id_cancel = -1;
                    LOGLOG("gavin: print job cancel 4 ...");
                    pthread_cancel(check_thread);
                    usleep(200000);
                    sm->mFinger.finger_cancel(sm->m_device_uri);
                    sm->check_result = Checked_Result_Abort_Print;

                    break;
                }
                gettimeofday(&tpend, 0);
                float timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
                timeuse /= 1000000;
                if((timeuse -(sm->m_timeout+1)) > 0)
                {
                    pthread_cancel(check_thread);
                    usleep(200000);
                    LOGLOG("gavin: print cancel 8 ...");
                    sm->mFinger.finger_cancel(sm->m_device_uri);
                    sm->check_result = Checked_Result_Cancel;
                    break;

                }

                if(sm->chenk_end)
                    break;
                usleep(200000);
            }
        }
        LOGLOG("check result:%s" ,buffer);

    }
    else if(isFingerEnable == 3)
    {
        LOGLOG("libtoec: finger is open, but not finger data");
        sm->check_result = Checked_Result_NoFinger;
        sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                ,sm->username ,sm->filename);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(!strcmp(buffer ,"resultok")){
            LOGLOG("gavin: result ok");
           // break;
        }
        return;
    }
    else
    {

        LOGLOG("libtoec: finger  not open");
        sm->check_result = Checked_Result_Disable;
        sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s",js->printer ,js->id, sm->check_result
                ,sm->username ,sm->filename);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(!strcmp(buffer ,"resultok")){
            LOGLOG("gavin: result ok");
           // break;
        }
        return;
    }

#if 0
    if(showDlg){
        LOGLOG("gavin: show Dlg...");

        int invalid_times = 0;
        sprintf(buffer ,"start://%s?jobid=%d" ,js->printer ,js->id);
        tc.writeThenRead(buffer ,sizeof(buffer));
        if(!strcmp(buffer ,"startok")){
            LOGLOG("gavin: show Dlg...OK");

             usleep(200000);
            while(1){

                sprintf(buffer ,"check://%s?jobid=%d" ,js->printer ,js->id);
                tc.writeThenRead(buffer ,sizeof(buffer));
                if(!strcmp(buffer ,"cancel")){
                        sm->check_result = Checked_Result_Cancel;
                        pthread_cancel(check_thread);
                        usleep(200000);
                        sm->mFinger.finger_cancel(sm->m_device_uri);
                         LOGLOG("gavin: show Dlg...cancel");
//                         while(1)
//                         {
//                             //wait thread finish
//                             if(sm->chenk_end)
//                                 break;
//                             usleep(10000);
//                         }
                        break;
                }else if(!strcmp(buffer ,"timeout")){
                        sm->check_result = Checked_Result_timeout;
                        pthread_cancel(check_thread);

                        usleep(200000);
                        sm->mFinger.finger_cancel(sm->m_device_uri);
                         LOGLOG("gavin: show Dlg...timeout");
//                        while(1)
//                        {
//                            //wait thread finish
//                            if(sm->chenk_end)
//                                break;
//                            usleep(10000);
//                        }

                        break;
                }else if(!strcmp(buffer ,"checking")){
                    invalid_times = 0;
                     //LOGLOG("gavin: show Dlg...checking");
                        usleep(200000);
                }else if(!strcmp(buffer ,"invalid")){
                    invalid_times++;
                     LOGLOG("gavin: show Dlg...invalid");
                    if(invalid_times >5)
                            break;
                    usleep(200000);

                }
                if(sm->chenk_end)
                {
                     LOGLOG("gavin: show Dlg...close");

                   // while(1)
                    {
                        sprintf(buffer ,"result://%s?jobid=%d&status=%d",js->printer ,js->id, sm->check_result);
                        tc.writeThenRead(buffer ,sizeof(buffer));
                        if(!strcmp(buffer ,"resultok")){
                            LOGLOG("gavin: show Dlg...close ok");
                           // break;
                        }
                    }
                    break;
                }

            }
        }
        else{
             LOGLOG("gavin: show Dlg...fail");
            //sm->check_result = Checked_Result_Fail;
             while(1)
             {
                 //wait thread finish
                 if(sm->chenk_end)
                     break;
                 usleep(200000);
             }
        }
        LOGLOG("check result:%s" ,buffer);
    }else{
        sm->check_result = Checked_Result_Disable;
    }
#endif
}

int FingerManager::checkFinger(const char* server_path ,int jobid ,const char* username ,const char* filename ,int use_finger_checking)
{
    LOGLOG("libtoec: start check finger job id:%d" ,jobid);
    LOGLOG("filterlib checkfinger user name:%s ,filename:%s" ,username ,filename);
    this->job_id = jobid;
    strcpy(this->username ,username);
    strcpy(this->filename ,filename);
    this->use_finger_checking = use_finger_checking;
//    this->username  = username;
//    this->filename  = filename;
    this->server_path = server_path;
    check_result = Checked_Result_invalidJobid;
    this->m_timeout = 30;
    QString path = QDir::homePath();

     LOGLOG("checkFinger: m_timeout = %d" ,m_timeout);
    cups_get_job(callback_getJob ,this);
    //return check_result;
    if(check_result == Checked_Result_OK ||check_result == Checked_Result_Disable)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
