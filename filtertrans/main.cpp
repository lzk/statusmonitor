#include <QApplication>
#include "commonapi.h"
#include "appconfig.h"
#include <signal.h>
#include "watcher.h"

void quit(int)
{
    LOGLOG("SIGINT quit");
    if(qApp)
        qApp->quit();
}

int main(int argc, char *argv[])
{
    if(AppConfig::initConfig()){
        LOGLOG("There has been a same app running!");
        return 0;
    }

    QApplication a(argc, argv);

    signal(SIGINT ,quit);
    signal(SIGHUP ,quit);

    Watcher* watcher = new Watcher;
    watcher->start();
//    pid_t pid = fork();
//    switch(pid)
//    {
//    case -1:
//        LOGLOG("fork failed");
//        exit(1);
//        break;
//    case 0:
//        execlp("tjgd1zsmui", "tjgd1zsmui" ,"-hide" ,0);
//        return 0;
//    default:
//        break;
//    }
    int ret = a.exec();
    delete watcher;
    AppConfig::exit_app();
    LOGLOG("app exit!");
    return ret;
}
