#include "appserver.h"
#include "commonapi.h"
#include "jkinterface.h"
#include "appconfig.h"
//#include <QProcess>
#include <unistd.h>
#include "statusthread.h"
#include <QUrl>
#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
#include <QSettings>
#include <signal.h>

#include "tomcat.h"
#include "filterlib.h"
AppServer::AppServer(const char* server_path ,QObject *parent)
    : QObject(parent)
//    ,statusThread(NULL)
    ,server_path(server_path)
{
//    trans_server.createServer(server_path);
    thread_server = new ServerThread(server_path);
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();

//    statusThread = new StatusThread();
//    statusThread->moveToThread(&thread);
//    connect(&thread ,SIGNAL(finished()) ,statusThread ,SLOT(deleteLater()));
//    connect(this ,SIGNAL(signal_set_device_id(const QString& ,const QString&)) ,statusThread ,SLOT(set_device_id(const QString& ,const QString&)));

//    thread.start();
//    statusThread->start();
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

    LOGLOG("restart server:%s" ,server_path.toLatin1().constData());
    thread_server = new ServerThread(server_path.toLatin1().constData());
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();
}

void AppServer::set_device_id(const QString& printer ,const QString& device_id)
{
    signal_set_device_id(printer ,device_id);
}

void AppServer::set_current_printer(const QString& printer)
{
//    if(statusThread)
//        statusThread->set_current_printer(printer);
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
    LOGLOG("cmd is:%s" ,cmd.toLatin1().constData());
    QString printer = QString(buffer).mid(index + strlen("://"));
    index = printer.indexOf('?');
    if(index > 0)
        printer = printer.left(index);
    LOGLOG("printer is:%s" ,printer.toLatin1().constData());
#if 1
    if(!cmd.compare("start")){

        int time_val;
        //QString homepath = QDir::homePath();
        QSettings setting("/usr/share/tjgd1z/setting.ini", QSettings::defaultFormat());
        bool ok;
        time_val = setting.value("TimeOut").toInt(&ok);
        if(!ok)
            time_val = 30;
        sprintf(buffer ,"startok:%d", time_val);

        cmd += QString("&time_val = %1").arg(time_val);
        pid_t pid = fork();
        switch(pid)
        {
        case -1:
            LOGLOG("fork failed");
            exit(1);
            break;
        case 0:{
            execlp("tjgd1zfmui", "tjgd1zfmui" ,cmd.toLatin1().constData() ,0);
//            QString _cmd("tjgd1zfmui ");
//            _cmd += cmd;
//            system(_cmd.toLatin1().constData());
            exit(0);
            break;
        }
        default:
            break;
        }

    }else if(!cmd.compare("check")){
//        int result = app_server->get_finger_result(jobid);
        int result = Checked_Result_invalidJobid;
        QSettings settings("/tmp/tjgd1zsmtmp.txt" ,QSettings::NativeFormat);
        QString str_jobid = QString("%1").arg(jobid);
        QString value = settings.value(str_jobid).toString();
        int ui_pid = 0;
        if(value.isEmpty()){
            result = Checked_Result_checking;
        }else{
            QStringList columns = value.split(",");
            result = columns.at(0).toInt();
            switch (result) {
            case 0:
                result = Checked_Result_checking;
                break;
            case 1:
                result = Checked_Result_Cancel;
                break;
            case 2:
                result = Checked_Result_timeout;
                break;
            default:
                result = Checked_Result_invalidJobid;
                break;
            }
            ui_pid = columns.at(1).toInt();
        }

        switch(result){
        case Checked_Result_Cancel:
            strcpy(buffer ,"cancel");
            if(ui_pid)
                kill(ui_pid ,9);
            break;
        case Checked_Result_timeout:
            strcpy(buffer ,"timeout");
            if(ui_pid)
                kill(ui_pid ,9);
            break;
        case Checked_Result_checking:
            strcpy(buffer ,"checking");
            break;
        case Checked_Result_invalidJobid:
        default:
            strcpy(buffer ,"invalid");
            if(ui_pid)
                kill(ui_pid ,9);
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
  #else

#if 0
    if(!cmd.compare("stcp")){
        app_server->set_current_printer(printer);
        strcpy(buffer ,"stcpok");
        return 0;
    }else if(!cmd.compare("dvid")){
//        QString device_id;
//        index = str.indexOf("deviceid=");
//        device_id = str.mid(index + strlen("deviceid="));
//        LOGLOG("device_id is:%s" ,device_id.toLatin1().constData());
//        app_server->set_device_id(printer ,device_id);
//        strcpy(buffer ,"didok");
//        return 0;
    }
#endif

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
    return 0;
}

void AppServer::client_connect(int fd)
{
    thread_server->get_trans_server()->readThenWrite(fd ,callback_Server ,this);
}

void AppServer::client_cmd(const QString & ,void* )
{

}
