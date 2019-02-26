#include <QCoreApplication>
#include "commonapi.h"
#include "appserver.h"
#include "appconfig.h"
#include <signal.h>
void quit(int)
{
    LOGLOG("SIGINT quit");
    if(qApp)
        qApp->quit();
}

int main(int argc, char *argv[])
{
    if(isRunning(SERVER_PATH)){
        return 0;
    }
    QCoreApplication a(argc, argv);

    signal(SIGINT ,quit);

    AppServer* app_server = new AppServer(SERVER_PATH);
    int ret = a.exec();
    delete app_server;
    return ret;
}
