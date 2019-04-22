#include "mainwindow.h"
#include <QApplication>
#include <signal.h>
#include <unistd.h>
#include <QSettings>
int g_jobid = 0;
int app_ret = 0;

void quit(int)
{
    if(qApp)
        qApp->quit();
}

int main(int argc, char *argv[])
{
    signal(SIGINT ,quit);

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/app_icon.png"));

    QStringList arguments = QCoreApplication::arguments();
    QString str;
    if(arguments.count() < 2)
        str = "start://testprinter?jobid=0&time_val=30";
    else
        str = arguments[1];
    MainWindow w(str);
    w.show();

    QSettings settings("/tmp/tjgd1zsmtmp.txt" ,QSettings::NativeFormat);
    QString value = QString("%1,%2").arg(app_ret).arg(getpid());
    settings.setValue(QString("%1").arg(g_jobid) ,value);

    a.exec();
    value = QString("%1,%2").arg(app_ret).arg(getpid());
    settings.setValue(QString("%1").arg(g_jobid) ,value);
    return 0;
}
