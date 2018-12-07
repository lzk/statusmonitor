#include "appserver.h"
#include "checkfingerdialog.h"
#include "fingerhandler.h"
#include "commonapi.h"
#include "jkinterface.h"
#include <QUrl>
#if QT_VERSION_MAJOR > 4
#include <QUrlQuery>
#endif
#include "clientthread.h"
AppServer::AppServer(const char* server_path ,QObject *parent)
    : QObject(parent)
{
//    trans_server.createServer(server_path);

    finger_handler = new FingerHandler;
    finger_handler->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,finger_handler ,SLOT(deleteLater()));
    thread.start();

    thread_server = new ServerThread(server_path);
    connect(thread_server ,SIGNAL(client_connect(int)) ,this ,SLOT(client_connect(int)));
    connect(thread_server ,SIGNAL(client_cmd(QString ,void*)) ,this ,SLOT(client_cmd(QString ,void*)));
    thread_server->start();
}

AppServer::~AppServer()
{
    delete thread_server;
    thread.quit();
    thread.wait();
}

void AppServer::client_connect(int fd)
{
    ClientThread* cth = new ClientThread(thread_server ,fd);
    connect(cth ,SIGNAL(signal_check_finger(int)) ,finger_handler ,SLOT(check_finger(int)));
    connect(finger_handler ,SIGNAL(check_finger_result(int,int)) ,cth ,SLOT(check_finger_result(int ,int)) ,Qt::DirectConnection);
    cth->start();
}

void AppServer::client_cmd(const QString &s ,void* para)
{
    LOGLOG("client cmd:%s" ,s.toLatin1().constData());
    QUrl url(s);
    QString cmd = url.scheme();
//    QString printer = url.host();
    int jobid;
#if QT_VERSION_MAJOR > 4
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
        connect(finger_handler ,SIGNAL(start_check_finger(int)) ,dialog ,SLOT(start_check_finger(int)));
        connect(finger_handler ,SIGNAL(active_window(int)) ,dialog ,SLOT(active_window(int)));
        dialog->show();
        finger_handler->active_current_jobid_window();
        wt->check_finger(jobid);
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

