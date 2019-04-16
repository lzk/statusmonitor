#include "appserver.h"
#include "checkfingerdialog.h"
#include "fingerhandler.h"
#include "commonapi.h"
#include "tomcat.h"
#include "jkinterface.h"
#include <QUrl>
#include <QDir>
#include <QSettings>
#include <QMessageBox>

#include "filterlib.h"

#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
#include "clientthread.h"
AppServer::AppServer(const char* server_path ,QObject *parent)
    : QObject(parent)
    ,server_path(server_path)
{
//    trans_server.createServer(server_path);

//    finger_handler = new FingerHandler;
//    finger_handler->moveToThread(&thread);
//    connect(&thread ,SIGNAL(finished()) ,finger_handler ,SLOT(deleteLater()));
//    thread.start();

    thread_server = new ServerThread(server_path);
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();
}

AppServer::~AppServer()
{
    delete thread_server;
//    thread.quit();
//    thread.wait();
}

void AppServer::restart_server()
{
    delete thread_server;

    thread_server = new ServerThread(server_path.toLatin1().constData());
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();
}

void AppServer::set_device_id(const QString& printer ,const QString& device_id)
{
    signal_set_device_id(printer ,device_id);
}

static int callback_Server(void* para,char* buffer,int bufsize)
{
    AppServer* app_server = (AppServer*)para;
    QString str(buffer);
    QUrl url(str);
    QString cmd = url.scheme();
    int jobid;
#if QT_VERSION > 0x050000
    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
#else
    jobid = QUrl(url).queryItemValue("jobid").toInt();
#endif
//    LOGLOG("filter cmd:%s" ,buffer);

    memset(buffer ,0 ,bufsize);
    if(!cmd.compare("start")){
        app_server->new_finger_dialog(jobid ,str);
        int time_val;
        //QString homepath = QDir::homePath();
        QSettings setting("/usr/share/tjgd1z/setting.ini", QSettings::defaultFormat());
        bool ok;
        time_val = setting.value("TimeOut").toInt(&ok);
        if(!ok)
            time_val = 30;
        //strcpy(buffer ,"startok");
        sprintf(buffer ,"startok:%d", time_val);
    }else if(!cmd.compare("check")){
        int result = app_server->get_finger_result(jobid);
        switch(result){
        case Checked_Result_Cancel:
            strcpy(buffer ,"cancel");
            app_server->delete_finger_dialog(jobid);
            break;
        case Checked_Result_timeout:
            strcpy(buffer ,"timeout");
            app_server->delete_finger_dialog(jobid);
            break;
        case Checked_Result_checking:
            strcpy(buffer ,"checking");
            break;
        case Checked_Result_invalidJobid:
        default:
            strcpy(buffer ,"invalid");
            app_server->delete_finger_dialog(jobid);
            break;
        }


    }else if(!cmd.compare("result")){
        int finger_checked_result;
#if QT_VERSION > 0x050000
        finger_checked_result = QUrlQuery(QUrl(url)).queryItemValue("status").toInt();
#else
        finger_checked_result = QUrl(url).queryItemValue("status").toInt();
#endif
        if(finger_checked_result != Checked_Result_Disable){
            app_server->delete_finger_dialog(jobid);
            if(finger_checked_result == Checked_Result_DevBusy)
            {
                QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("设备忙，当前打印作业将被取消！"));
                msg.setStandardButtons (QMessageBox::Ok);
                msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
                msg.exec ();
            }
            else if(finger_checked_result == Checked_Result_Fail)
            {
                QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("指纹验证失败，打印作业将被取消！"));
                msg.setStandardButtons (QMessageBox::Ok);
                msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
                msg.exec ();
            }
            else if(finger_checked_result == Checked_Result_NoFinger)
            {
                QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("设备用户列表为空，无法进行指纹验证，当前打印作业将被取消！"));
                msg.setStandardButtons (QMessageBox::Ok);
                msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
                msg.exec ();
            }
            else if(finger_checked_result == Checked_Result_timeout || finger_checked_result == Checked_Result_Cancel|| finger_checked_result == Checked_Result_invalidJobid)
            {
                QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("指纹验证被用户取消，打印作业也将被取消！"));
                msg.setStandardButtons (QMessageBox::Ok);
                msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
                msg.exec ();
            }
        }
        strcpy(buffer ,"resultok");
#if 0
        QVariant value;
        appSettings("record" ,value ,QVariant(false));
        bool record_list = value.toBool();
        if(record_list){
            LOGLOG("record to file list");
            Job_history job;
            job.id = jobid;
            job.is_finger_enable = (finger_checked_result != Checked_Result_Disable) ?1 :0;
            job.is_finger_checked = (finger_checked_result == Checked_Result_OK) ?1 :0;
#if QT_VERSION > 0x050000
        job.username = QUrlQuery(QUrl(url)).queryItemValue("username");
        job.filename = QUrlQuery(QUrl(url)).queryItemValue("filename");
#else
        job.username = QUrl(url).queryItemValue("username");
        job.filename = QUrl(url).queryItemValue("filename");
#endif
            Tomcat::save_job_history(&job);
        }else{
            LOGLOG("do not record to file list");
        }
#endif
    }else if(!cmd.compare("dvid")){
        int index;
        index = str.indexOf("://");
//        cmd = str.left(index);
//        LOGLOG("cmd is:%s" ,cmd.toLatin1().constData());
        QString printer = QString(buffer).mid(index + strlen("://"));
        index = printer.indexOf('?');
        if(index > 0)
            printer = printer.left(index);
        index = str.indexOf("deviceid=");
        QString device_id;
        device_id = str.mid(index + strlen("deviceid="));
        LOGLOG("device_id is:%s" ,device_id.toLatin1().constData());
        app_server->set_device_id(printer ,device_id);
        strcpy(buffer ,"didok");
    }
    return 0;
}

void AppServer::client_connect(int fd)
{
#if 0
    ClientThread* cth = new ClientThread(thread_server ,fd);
//    connect(cth ,SIGNAL(signal_check_finger(int)) ,finger_handler ,SLOT(check_finger(int)));
//    connect(finger_handler ,SIGNAL(check_finger_result(int,int)) ,cth ,SLOT(check_finger_result(int ,int)) ,Qt::DirectConnection);
    cth->start();
  #endif
    thread_server->trans_server.readThenWrite(fd ,callback_Server ,this);
}

void AppServer::client_cmd(const QString &s ,void* para)
{
    LOGLOG("client cmd:%s" ,s.toLatin1().constData());
    QUrl url(s);
    QString cmd = url.scheme();
//    QString printer = url.host();
    int jobid;

#if QT_VERSION > 0x050000
    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
#else
    jobid = QUrl(url).queryItemValue("jobid").toInt();
#endif
    ClientThread* wt = (ClientThread*)para;
    if(!cmd.compare("check")){
        CheckFingerDialog* dialog = new CheckFingerDialog(s);
        connect(dialog ,SIGNAL(cancel_job(int)) ,wt ,SLOT(cancel(int)) ,Qt::DirectConnection);
        connect(dialog ,SIGNAL(job_timeout(int)) ,wt ,SLOT(timeout(int)) ,Qt::DirectConnection);
        connect(wt ,SIGNAL(delete_dialog()) ,dialog ,SLOT(deleteLater()));
//        connect(finger_handler ,SIGNAL(start_check_finger(int)) ,dialog ,SLOT(start_check_finger(int)));
//        connect(finger_handler ,SIGNAL(active_window(int)) ,dialog ,SLOT(active_window(int)));
        dialog->show();
//        finger_handler->active_current_jobid_window();
//        wt->check_finger(cmd);
    }else if(!cmd.compare("record")){
        QVariant value;
        appSettings("record" ,value ,QVariant(false));
        bool record_list = value.toBool();
        if(record_list){
            LOGLOG("record to file list");
            wt->client_cmd_result("record");
        }else{
            LOGLOG("do not record to file list");
            wt->client_cmd_result("donotrecord");
        }
    }
}

void AppServer::new_finger_dialog(int id ,const QString& s)
{
    foreach(FingerResult_struct fr ,finger_result_list){
        if(id == fr.id){
            LOGLOG("id:%d dialog exist!" ,id);
            return;
        }
    }

    CheckFingerDialog* dialog;
    dialog = new CheckFingerDialog(s);
    connect(dialog ,SIGNAL(cancel_job(int)) ,this ,SLOT(cancel(int)));
    connect(dialog ,SIGNAL(job_timeout(int)) ,this ,SLOT(timeout(int)));
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnTopHint);
    dialog->show();
//    dialog->raise();

    FingerResult_struct finger_result;
    finger_result.id = id;
    finger_result.result = Checked_Result_checking;
    finger_result.dialog = dialog;
    finger_result_list << finger_result;
}

void AppServer::delete_finger_dialog(int id)
{
    int index = -1;
    for(int i = 0 ; i < finger_result_list.count() ;i++){
        if(id == finger_result_list[i].id){
            index = i;
            if(finger_result_list[i].dialog){
                delete finger_result_list[i].dialog;
                finger_result_list[i].dialog = NULL;
            }
            break;
        }
    }
    if(index >= 0)
        finger_result_list.removeAt(index);
}

void AppServer::cancel(int id)
{
    for(int i = 0 ; i < finger_result_list.count() ;i++){
        if(id == finger_result_list[i].id){
            finger_result_list[i].result = Checked_Result_Cancel;
            if(finger_result_list[i].dialog){
                delete finger_result_list[i].dialog;
                finger_result_list[i].dialog = NULL;
            }
            break;
        }
    }

}

void AppServer::timeout(int id)
{
    for(int i = 0 ; i < finger_result_list.count() ;i++){
        if(id == finger_result_list[i].id){
            finger_result_list[i].result = Checked_Result_timeout;
            if(finger_result_list[i].dialog){
                delete finger_result_list[i].dialog;
                finger_result_list[i].dialog = NULL;
            }
            break;
        }
    }
}

int AppServer::get_finger_result(int id)
{
    int result = Checked_Result_invalidJobid;
    for(int i = 0 ; i < finger_result_list.count() ;i++){
        if(id == finger_result_list[i].id){
            result = finger_result_list[i].result;
            break;
        }
    }
    return result;
}
