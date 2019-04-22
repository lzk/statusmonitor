#include "watcher.h"
#include "tomcat.h"
#include "appserver.h"
#include "uiconfig.h"
#include "statusthread.h"
extern AppServer* app_server;
Watcher::Watcher(QObject *parent)
    : StatusWatcher(parent)
{
    connect(app_server ,SIGNAL(signal_set_device_id(QString,QString)) ,this ,SLOT(set_device_id(QString,QString)));
    connect(this ,SIGNAL(server_restart()) ,app_server ,SLOT(restart_server()));

}

Watcher::~Watcher()
{
}

void Watcher::watcher_job()
{
    if(!is_app_running(SERVER_PATH_STM)){
        server_restart();
    }

    //update job history info
//    Tomcat::update_job_history();
}
