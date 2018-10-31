#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "uinterface.h"
UInterface* gUInterface;
#include <sys/wait.h>
#include "trans.h"

void quit(int)
{
    LOGLOG("SIGINT quit");
    if(qApp)
        qApp->quit();
}

bool isRunning()
{
    bool running = true;
    Trans_Client tc;
    int ret = tc.tryConnectToServer();
    switch (ret) {
    case 0:
        LOGLOG("There has been a same app running!");
        break;
    case -2:
        running = false;
        break;
    default:
        LOGLOG("There is something error!");
        break;
    }
    return running;
}

int main(int argc, char *argv[])
{
    if(isRunning()){
        return 0;
    }

    signal(SIGINT ,quit);
#ifdef Q_WS_X11
    qputenv("LIBOVERLAY_SCROLLBAR", 0);
#endif
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/app_icon.png"));

#if QT_VERSION_MAJOR < 5
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif
    UInterface uinterface;
    gUInterface = &uinterface;
//    qRegisterMetaType<QVariant>("QVariant");
    MainWindow w;
    QStringList arguments = QCoreApplication::arguments();
    if(!arguments.contains("-hide"))
        w.show();
    
    return a.exec();
}
