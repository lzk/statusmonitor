#include <QCoreApplication>
#include "commonapi.h"
#include "appserver.h"
#include "appconfig.h"
#include <signal.h>
#include "watcher.h"
void quit(int)
{
    LOGLOG("SIGINT quit");
    if(qApp)
        qApp->quit();
}
AppServer* app_server;

//#include <unistd.h>
int main(int argc, char *argv[])
{
    if(is_app_running(SERVER_PATH)){
        LOGLOG("There has been a same app running!");
        return 0;
    }

//    int result = daemon(0 ,2);
//    if(!result){
//        LOGLOG("daemon success!");
//    }

    AppConfig::initConfig();

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
