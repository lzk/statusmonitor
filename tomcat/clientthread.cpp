#include "clientthread.h"
#include <unistd.h>
#include "jkinterface.h"
#include "fingerhandler.h"
#include "commonapi.h"
#include "tomcat.h"
#include <QUrl>
#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
enum{
    Checked_Result_OK,
    Checked_Result_Disable,
    Checked_Result_Fail,
    Checked_Result_Cancel,
    Checked_Result_invalidJobid,
    Checked_Result_timeout,
};

int callback_Server(void* para,char* buffer,int bufsize)
{
    ClientThread* wt = (ClientThread*)para;
    QUrl url(buffer);
    QString cmd = url.scheme();
    QString printer = url.host();
    int jobid;
#if QT_VERSION > 0x050000
    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
#else
    jobid = QUrl(url).queryItemValue("jobid").toInt();
#endif
    LOGLOG("filter check printer:%s\n\t jobid:%d" ,printer.toLatin1().constData() ,jobid);
    wt->jobid = jobid;
    int timeout = 4000;
    if(!cmd.compare("check")){
        wt->check_finger(buffer);
        while (!wt->trans_back() && !wt->m_cancel && !wt->m_timeout && timeout --) {
            usleep(10000);
        }
        memset(buffer ,0 ,bufsize);
        strcpy(buffer ,wt->result().toLatin1().constData());
    }else if(!cmd.compare("record")){
        timeout = 100;
        wt->send_cmd(buffer);
        while (!wt->trans_back() && timeout --) {
            usleep(10000);
        }
        memset(buffer ,0 ,bufsize);
        strcpy(buffer ,wt->result().toLatin1().constData());
    }
    return 0;
}

ClientThread::ClientThread(ServerThread* _server ,int _fd ,QObject *parent)
        :QThread(parent)
          ,server(_server)
    ,fd(_fd)
{
    finger_handler = new FingerHandler(this);
    finger_handler->moveToThread(this);
    connect(this ,SIGNAL(finished()) ,finger_handler ,SLOT(deleteLater()));
//    connect(this ,SIGNAL(signal_check_finger(QString)) ,finger_handler ,SLOT(check_finger(QString)) ,Qt::DirectConnection);
//    connect(finger_handler ,SIGNAL(check_finger_result(int,int)) ,this ,SLOT(check_finger_result(int ,int)) ,Qt::DirectConnection);
}

void ClientThread::run(){
    print_check_flag = false;
    server->trans_server.readThenWrite(fd ,callback_Server ,this);
    if(print_check_flag){
        QVariant value;
        appSettings("record" ,value ,QVariant(false));
        bool record_list = value.toBool();
        if(record_list){
            LOGLOG("record to file list");
            Job_history job;
            job.id = jobid;
            job.result = finger_checked_result;
            Tomcat::save_job_history(&job);
        }else{
            LOGLOG("do not record to file list");
        }
    }
}

void ClientThread::check_finger_result(int jobid ,int result)
{
    if(jobid != this->jobid)
        return;
    switch (result) {
    case Checked_Result_Disable:
        m_result = "disable";
        m_trans_back = 1;
        break;
        case Checked_Result_Fail:
        m_result = "fail";
        m_trans_back = 0;//waiting for try to timeout
        break;
        case Checked_Result_Cancel:
        m_result = "cancel";
        m_trans_back = 1;
        break;
        case Checked_Result_invalidJobid:
        m_result = "invalid";
        m_trans_back = 1;
        break;
        case Checked_Result_timeout:
        m_result = "timeout";
        m_trans_back = 1;
        break;
    case Checked_Result_OK:
        m_result = "ok";
        m_trans_back = 1;
        break;
    default:
        m_result = "error";
        m_trans_back = 1;
        break;
    }
    LOGLOG("result of jobid %d:%s" ,jobid ,m_result.toLatin1().constData());
    delete_dialog();
    print_check_flag = true;
    finger_checked_result = result;
}

void ClientThread::send_cmd(const QString& cmd)
{
    m_result = "ok";
    m_trans_back = 0;
    m_cancel = false;
    m_timeout = false;
    server->client_cmd(cmd ,(void*)this);
}

void ClientThread::check_finger(const QString& cmd)
{
    int result;
    result = finger_handler->is_finger_enable();
    if(!result){
        send_cmd(cmd);//dispaly dialog
//        signal_check_finger(cmd);
        QUrl url(cmd);
        QString printer = url.host();
        char uri[256];
        cups_get_device_uri(printer.toLatin1().constData() ,uri);
        result = finger_handler->check_finger(uri ,jobid);
    }
    check_finger_result(jobid ,result);
}

void ClientThread::cancel(int jobid)
{
    m_result = "cancel";
    m_cancel = true;
    finger_handler->cancel();
}

void ClientThread::timeout(int jobid)
{
    m_result = "timeout";
    m_timeout = true;
    finger_handler->cancel();
}

void ClientThread::client_cmd_result(const QString &s)
{
    m_result = s;
    m_trans_back = 1;
}
