#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "uinterface.h"
UInterface* gUInterface;
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

    ServerThread* thread_server = new ServerThread(SERVER_PATH);
    thread_server->start();
//    StatusThread* statusThread = new StatusThread;
//    statusThread->start();

    signal(SIGINT ,quit);
#ifdef Q_WS_X11
    qputenv("LIBOVERLAY_SCROLLBAR", 0);
#endif
    QApplication a(argc, argv);
//    a.setWindowIcon(QIcon(":/image/app_icon.png"));

#if QT_VERSION_MAJOR < 5
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif

    gUInterface = new UInterface;

    MainWindow w;

    QStringList arguments = QCoreApplication::arguments();
    if(!arguments.contains("-hide"))
        w.show();

    int ret = a.exec();
    delete thread_server;
//    delete statusThread;
    delete gUInterface;
    return ret;
}
