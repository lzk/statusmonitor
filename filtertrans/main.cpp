#include <QCoreApplication>
#include "commonapi.h"
#include "appserver.h"
#include "appconfig.h"
#include <signal.h>
#include "watcher.h"
#include <unistd.h>

void quit(int)
{
    LOGLOG("SIGINT quit");
    if(qApp)
        qApp->quit();
}
AppServer* app_server;

int main(int argc, char *argv[])
{
    AppConfig::initConfig();
    if(is_app_running(SERVER_PATH)){
        LOGLOG("There has been a same app running!");
        return 0;
    }

#ifndef DEBUG_TO_STDERR
    //release as deaemon
    int result = daemon(0 ,0);
    if(!result){
        LOGLOG("daemon success!");
    }
#endif

    QCoreApplication a(argc, argv);

    signal(SIGINT ,quit);

    app_server = new AppServer(SERVER_PATH);
    Watcher* watcher = new Watcher;
    watcher->start();
    int ret = a.exec();
    delete watcher;
    delete app_server;
    AppConfig::exit_app();
    return ret;
}
