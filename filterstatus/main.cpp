#include <QCoreApplication>
#include "filterstatusthread.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FilterStatusThread* fst = new FilterStatusThread;
    fst->start();
    int ret = a.exec();
    delete fst;
    return ret;
}
