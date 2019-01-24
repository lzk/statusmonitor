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
#include <QFile>
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

    signal(SIGINT ,quit);
#ifdef Q_WS_X11
    qputenv("LIBOVERLAY_SCROLLBAR", 0);
#endif
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/app_icon.png"));

    AppServer* app_server = new AppServer(SERVER_PATH);

#if QT_VERSION < 0x050000
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif

    QStringList arguments = QCoreApplication::arguments();
    if(arguments.contains("-test"))
        use_status_thread = false;

    //worker run first,then show ui
    gUInterface = new UInterface;

    MainWindow* w = new MainWindow;

    if(!arguments.contains("-hide"))
        w->show();
    
    int ret = a.exec();
    delete w;
    delete app_server;
    delete gUInterface;
    QFile::remove(status_filename);
    QFile::remove(lockfile);
    return ret;
}
