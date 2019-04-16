#include "mainwindow.h"
#include <QApplication>

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

    a.exec();
    return app_ret;
}
