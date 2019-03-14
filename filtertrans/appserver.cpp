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
AppServer::AppServer(const char* server_path ,QObject *parent)
    : QObject(parent)
    ,server_path(server_path)
{
//    trans_server.createServer(server_path);
    thread_server = new ServerThread(server_path);
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();

    statusThread = new StatusThread();
    statusThread->start();
}

AppServer::~AppServer()
{
    if(statusThread)
        delete statusThread;

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

static int callback_Server(void* para ,char* buffer,int bufsize)
{
    AppServer* app_server = (AppServer*)para;
//    QUrl url(buffer);
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
    if(!cmd.compare("stcp")){
        app_server->statusThread->set_current_printer(printer);
        strcpy(buffer ,"stcpok");
        return 0;
    }else if(!cmd.compare("dvid")){
        QString device_id;
//#if QT_VERSION > 0x050000
//    device_id = QUrlQuery(QUrl(url)).queryItemValue("deviceid");
//#else
//    device_id = QUrl(url).queryItemValue("deviceid");
//#endif
        index = str.indexOf("deviceid=");
        device_id = str.right(index + strlen("deviceid="));
        LOGLOG("device_id is:%s" ,device_id.toLatin1().constData());
        app_server->statusThread->set_device_id(printer ,device_id);
        strcpy(buffer ,"didok");
        return 0;
    }

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
            execlp("tjgd1zsmui", "tjgd1zsmui" ,"-hide" ,0);
            break;
        default:
            sleep(1);
            tc.writeThenRead(buffer ,bufsize);
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

