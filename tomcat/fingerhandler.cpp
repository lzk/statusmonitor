#include "fingerhandler.h"
#include "finger.h"
#include "filterlib.h"
#include <QThread>
#include "jkinterface.h"
#include "clientthread.h"
#include <QUrl>
#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
FingerHandler::FingerHandler(ClientThread *_cth)
    : cth(_cth)
//    ,jobid(0)
{

}

void FingerHandler::cancel()
{
    LOGLOG("cancel finger check");
}

int FingerHandler::is_finger_enable()
{
    int result = Checked_Result_OK;
    //result = finger_isEnabled() ?Checked_Result_OK :Checked_Result_Disable;
    return result;
}

int FingerHandler::check_finger(const char* uri ,int id)
{
    int result;
    if(!cth)
        return -1;
#if 0
    while(true){
        if(cth->m_cancel){
            result = Checked_Result_Cancel;
            break;
        }

        if(cth->m_timeout){
            result = Checked_Result_timeout;
            break;
        }

//        if(!finger_check(uri)){
//            result = Checked_Result_Fail;
//        }else{
//            result = Checked_Result_OK;
//            break;
//        }
        QThread::usleep(10000);
    }
#endif
    result = Checked_Result_checking;
    if(cth->m_cancel){
        result = Checked_Result_Cancel;
    }

    if(cth->m_timeout){
        result = Checked_Result_timeout;
    }
    return result;
}

//void FingerHandler::check_finger(const QString& cmd)
//{
//    QUrl url(cmd);
//    QString printer = url.host();
//    int jobid;
//#if QT_VERSION > 0x050000
//    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
//#else
//    jobid = QUrl(url).queryItemValue("jobid").toInt();
//#endif
//    char uri[256];
//    cups_get_device_uri(printer.toLatin1().constData() ,uri);
//    LOGLOG("finger handler check %d begin" ,jobid);
//    this->jobid = jobid;
//    start_check_finger(jobid);
//    int result;
//    result = finger_isEnabled() ?Checked_Result_OK :Checked_Result_Disable;
//    if(!result){
////        ClientThread* cth = static_cast<ClientThread* >(sender());
//        if(!cth)
//            return;
//        while(true){
//            if(cth->m_cancel){
//                result = Checked_Result_Cancel;
//                check_finger_result(jobid ,result);
//                break;
//            }

//            if(cth->m_timeout){
//                result = Checked_Result_timeout;
//                check_finger_result(jobid ,result);
//                break;
//            }

//            if(!finger_check(uri)){
//                result = Checked_Result_Fail;
////                check_finger_result(jobid ,result);
//            }else{
//                result = Checked_Result_OK;
//                check_finger_result(jobid ,result);
//                break;
//            }
//            QThread::usleep(10000);
//        }
//    }else{
//        check_finger_result(jobid ,result);
//    }
//}

//void FingerHandler::active_current_jobid_window()
//{
//    if(jobid > 0)
//        active_window(jobid);
//}
