#include <QCoreApplication>

#include <sys/wait.h>
#include "uiconfig.h"
#include "serverthread.h"
#include "commonapi.h"
#include "statusthread.h"

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
    UIConfig::initConfig();

    ServerThread thread_server(SERVER_PATH);
    thread_server.start();
    StatusThread statusThread;
    statusThread.start();

    signal(SIGINT ,quit);

    QCoreApplication a(argc, argv);

    return a.exec();
}
