#include "fingerhandler.h"
#include "finger.h"
#include "filter_check_finger.h"
#include <QThread>
#include "jkinterface.h"
#include "clientthread.h"
FingerHandler::FingerHandler(QObject *parent)
    : QObject(parent)
    ,jobid(0)
{

}

void FingerHandler::check_finger(int jobid)
{
    LOGLOG("finger handler check %d begin" ,jobid);
    this->jobid = jobid;
    start_check_finger(jobid);
    int result;
    result = finger_isEnabled() ?Checked_Result_OK :Checked_Result_Disable;
    if(!result){
        ClientThread* cth = static_cast<ClientThread* >(sender());
        if(!cth)
            return;
        while(true){
            if(cth->m_cancel){
                result = Checked_Result_Cancel;
                check_finger_result(jobid ,result);
                break;
            }

            if(cth->m_timeout){
                result = Checked_Result_timeout;
                check_finger_result(jobid ,result);
                break;
            }

            if(!finger_check()){
                result = Checked_Result_Fail;
//                check_finger_result(jobid ,result);
            }else{
                result = Checked_Result_OK;
                check_finger_result(jobid ,result);
                break;
            }
            QThread::usleep(10000);
        }
    }else{
        check_finger_result(jobid ,result);
    }
}

void FingerHandler::active_current_jobid_window()
{
    if(jobid > 0)
        active_window(jobid);
}
