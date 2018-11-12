#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include "uinterface.h"
UInterface* gUInterface;
#include <sys/wait.h>
#include "uiconfig.h"
#include "serverthread.h"

void quit(int)
{
    LOGLOG("SIGINT quit");
    if(qApp)
        qApp->quit();
}

bool isRunning(const char* server_path);

int main(int argc, char *argv[])
{
    if(isRunning(SERVER_PATH)){
        return 0;
    }
    ServerThread thread_server(SERVER_PATH);

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
    w.connect(&thread_server ,SIGNAL(client_cmd(QString)) ,&w ,SLOT(client_cmd(QString)));

    QStringList arguments = QCoreApplication::arguments();
    if(!arguments.contains("-hide"))
        w.show();
    
    return a.exec();
}
