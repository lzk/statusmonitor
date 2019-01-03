#ifndef STATUSTHREAD_H
#define STATUSTHREAD_H

#include <QThread>
#include "jkinterface.h"
#include "commonapi.h"
#include "cupsmanager.h"
#include "statusmanager.h"

class StatusThread : public QThread
{
    Q_OBJECT
public:
    explicit StatusThread(QObject *parent = NULL);
    ~StatusThread();

    void run();

    QList<Printer_struct> printers;
    QStringList printerlist;

    bool abort;
private:
    DeviceManager* devicemanager;
    CupsManager cupsmanager;

public:
    StatusManager statusmanager;
};

#endif // STATUSTHREAD_H
