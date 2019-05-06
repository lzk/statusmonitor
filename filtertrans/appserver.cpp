#include "appserver.h"
#include "commonapi.h"
#include "jkinterface.h"
#include "appconfig.h"
//#include <QProcess>
#include <unistd.h>
#include <QUrl>
#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
#include <QSettings>
#include <signal.h>

#include "tomcat.h"
#include "filterlib.h"
QString get_result(int jobid)
{
    QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
    QString str_jobid = QString("%1").arg(jobid);
    QString value = settings.value(str_jobid).toString();
    return value;
}

AppServer::AppServer(const char* server_path ,QObject *parent)
    : QObject(parent)
    ,server_path(server_path)
{
    thread_server = new ServerThread(server_path);
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();

}

AppServer::~AppServer()
{
    delete thread_server;
}

void AppServer::restart_server()
{
    delete thread_server;

    LOGLOG("restart server:%s" ,server_path.toLatin1().constData());
    thread_server = new ServerThread(server_path.toLatin1().constData());
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();
}

static int callback_Server(void* para ,char* buffer,int bufsize)
{
    AppServer* app_server = (AppServer*)para;
    QUrl url(buffer);
    int jobid;
#if QT_VERSION > 0x050000
    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
#else
    jobid = QUrl(url).queryItemValue("jobid").toInt();
#endif
    QString cmd;
//    cmd = url.scheme();
//    QString printer = url.host(QUrl::PrettyDecoded);
    QString str(buffer);
    int index;
    index = str.indexOf("://");
    cmd = str.left(index);
//    LOGLOG("cmd is:%s" ,cmd.toLatin1().constData());
    QString printer = QString(buffer).mid(index + strlen("://"));
    index = printer.indexOf('?');
    if(index > 0)
        printer = printer.left(index);
//    LOGLOG("printer is:%s" ,printer.toLatin1().constData());
#if 1
    if(!cmd.compare("start")){
        LOGLOG("jobid:%d start check finger" ,jobid);

        int time_val;
        //QString homepath = QDir::homePath();
        QSettings setting("/usr/share/tjgd1z/setting.ini", QSettings::defaultFormat());
        bool ok;
        time_val = setting.value("TimeOut").toInt(&ok);
        if(!ok)
            time_val = 30;
        sprintf(buffer ,"startok:%d", time_val);

        QString tmp_cmd(str);
        tmp_cmd += QString("&time_val=%1").arg(time_val);
        LOGLOG("tjgd1zfmui cmd : %s" ,tmp_cmd.toLatin1().constData());
        pid_t pid = fork();
        switch(pid)
        {
        case -1:
            LOGLOG("fork failed");
            exit(1);
            break;
        case 0:{
            execl("/usr/share/tjgd1z/tjgd1zfmui", "tjgd1zfmui" ,tmp_cmd.toLatin1().constData() ,0);
//            execlp("tjgd1zfmui", "tjgd1zfmui" ,tmp_cmd.toLatin1().constData() ,0);
            exit(0);
            break;
        }
        default:
            break;
        }


    }else if(!cmd.compare("check")){
//        int result = app_server->get_finger_result(jobid);
        int result = Checked_Result_invalidJobid;
        QString value = get_result(jobid);
        int ui_pid = 0;
        if(value.isEmpty()){
            result = Checked_Result_checking;
        }else{
//            LOGLOG("tjgd1zfmui result %d: %s" ,jobid ,value.toLatin1().constData());
            QStringList columns = value.split(",");
            result = columns.at(0).toInt();
            ui_pid = columns.at(1).toInt();
        }

        switch(result){
        case Checked_Result_Cancel:
            strcpy(buffer ,"cancel");
//            app_server->delete_finger_dialog(ui_pid);
            break;
        case Checked_Result_timeout:
            strcpy(buffer ,"timeout");
//            app_server->delete_finger_dialog(ui_pid);
            break;
        case Checked_Result_checking:
            strcpy(buffer ,"checking");
            break;
        case Checked_Result_invalidJobid:
        default:
            strcpy(buffer ,"invalid");
//            app_server->delete_finger_dialog(ui_pid);
            break;
        }


    }else if(!cmd.compare("result")){

        int finger_checked_result;
#if QT_VERSION > 0x050000
        finger_checked_result = QUrlQuery(QUrl(url)).queryItemValue("status").toInt();
#else
        finger_checked_result = QUrl(url).queryItemValue("status").toInt();
#endif
        strcpy(buffer ,"resultok");
        LOGLOG("jobid:%d check finger result:%d" ,jobid ,finger_checked_result);

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

        QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
        QString str_jobid = QString("%1").arg(jobid);
        QString str_value = settings.value(str_jobid).toString();
        int ui_pid = 0;
        if(str_value.isEmpty()){
        }else{
//            LOGLOG("tjgd1zfmui result %d: %s" ,jobid ,value.toLatin1().constData());
            QStringList columns = str_value.split(",");
            ui_pid = columns.at(1).toInt();
            str_value = QString("%1,%2").arg(finger_checked_result).arg(ui_pid);
            settings.setValue(QString("%1").arg(jobid) ,str_value);
//            app_server->delete_finger_dialog(ui_pid);
            if(ui_pid){
                kill(ui_pid ,34 + finger_checked_result);
                LOGLOG("kill %d %d" ,ui_pid ,34 + finger_checked_result);
            }
        }
    }

  #else

    Trans_Client tc(SERVER_PATH_STM);
    if(tc.writeThenRead(buffer ,bufsize)){
        pid_t pid = fork();
        switch(pid)
        {
        case -1:
            LOGLOG("fork failed");
            exit(1);
            break;
        case 0:
//            str = get_string_from_shell_cmd("who am i" ,1);
//            LOGLOG("who am i:\n%s" ,str.toLatin1().constData());
//            str = get_string_from_shell_cmd("who" ,1);
//            LOGLOG("who:\n%s" ,str.toLatin1().constData());
//            str = get_string_from_shell_cmd("whoami");
//            LOGLOG("whoami:\n%s" ,str.toLatin1().constData());
//            str = getenv("DISPLAY");
//            LOGLOG("dislpay is %s" ,str.toLatin1().constData());
//            if(str.isEmpty()){
//                setenv("DISPLAY" ,":0.0" ,0);
//            }
//            str = getenv("DISPLAY");
//            LOGLOG("now dislpay is %s" ,str.toLatin1().constData());
            execlp("tjgd1zsmui", "tjgd1zsmui" ,"-hide" ,0);
            exit(0);
            break;
        default:
            sleep(1);
            tc.writeThenRead(buffer ,bufsize);
            break;
        }
    }
    if(!cmd.compare("result")){
        int finger_checked_result;
    #if QT_VERSION > 0x050000
        finger_checked_result = QUrlQuery(QUrl(url)).queryItemValue("status").toInt();
    #else
        finger_checked_result = QUrl(url).queryItemValue("status").toInt();
    #endif
        strcpy(buffer ,"resultok");

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
    }
#endif
    //start tjgd1zsmui
    const char* app_locker_file = "/tmp/.tjgd1zsmui_locker";
    FileLocker app_file_locker;
    if(!app_file_locker.trylock(app_locker_file)){
        app_file_locker.unlock();
        LOGLOG("tjgd1zsmui not locked!");
        pid_t pid = fork();
        switch(pid)
        {
        case -1:
            LOGLOG("fork failed");
            exit(1);
            break;
        case 0:
            execlp("tjgd1zsmui", "tjgd1zsmui" ,"-hide" ,0);
            exit(0);
            break;
        default:
            break;
        }
    }
    return 0;
}

void AppServer::client_connect(int fd)
{
    thread_server->get_trans_server()->readThenWrite(fd ,callback_Server ,this);
}

void AppServer::client_cmd(const QString & ,void* )
{

}

void AppServer::delete_finger_dialog(int id)
{
    if(id){
        kill(id ,34);
        LOGLOG("kill %d" ,id);
    }
}
