#include "watcher.h"
#include "appserver.h"
#include "appconfig.h"
#include "commonapi.h"
extern AppServer* app_server;
Watcher::Watcher(QObject *parent)
    : QThread(parent)
    ,abort(false)
{
    connect(this ,SIGNAL(server_restart()) ,app_server ,SLOT(restart_server()));

}

Watcher::~Watcher()
{
    abort = true;
    while(abort)usleep(1000);
}

void Watcher::run()
{
    forever{
        if (abort)
            break;
        timerOut();
        usleep(100*1000);
    }
    abort = false;
}

void Watcher::timerOut()
{
    static int count = 0;
    count ++;
    if(count == 100)
        count = 0;
    if((count % 10 == 0) && !is_app_running(SERVER_PATH)){
        server_restart();
    }
}
