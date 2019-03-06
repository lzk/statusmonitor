#ifndef FILTERSTATUSTHREAD_H
#define FILTERSTATUSTHREAD_H

#include <QThread>
//#include "cupsmanager.h"
//#include "statusmanager.h"
class FilterStatusThread : public QThread
{
    Q_OBJECT
public:
    explicit FilterStatusThread(QObject *parent = NULL);
    ~FilterStatusThread();

    void run();

private:
    bool abort;
//    CupsManager cupsmanager;
//    StatusManager statusmanager;

//    PRINTER_STATUS status;
};

#endif // FILTERSTATUSTHREAD_H
