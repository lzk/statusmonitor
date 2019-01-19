#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "uinterface.h"
UInterface* gUInterface;
#include <sys/wait.h>
#include "uiconfig.h"
#include "commonapi.h"
#include "statusthread.h"
#include "appserver.h"

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

//    ServerThread thread_server(SERVER_PATH);
//    StatusThread statusThread;
//    statusThread.start();

    signal(SIGINT ,quit);
#ifdef Q_WS_X11
    qputenv("LIBOVERLAY_SCROLLBAR", 0);
#endif
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/app_icon.png"));

    AppServer* app_server = new AppServer(SERVER_PATH);

#if QT_VERSION_MAJOR < 5
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif

    gUInterface = new UInterface;
//    qRegisterMetaType<QVariant>("QVariant");

    MainWindow* w = new MainWindow;

    QStringList arguments = QCoreApplication::arguments();
    if(!arguments.contains("-hide"))
        w->show();
    
    int ret = a.exec();
    delete app_server;
    delete gUInterface;
    delete w;
    return ret;
}
