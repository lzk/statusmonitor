#ifndef STATUSTHREAD_H
#define STATUSTHREAD_H

#include <QThread>
#include "jkinterface.h"
#include "commonapi.h"
#include "cupsmanager.h"
#include "statusmanager.h"
#include <QMutex>
class StatusThread : public QThread
{
    Q_OBJECT
public:
    explicit StatusThread(QObject *parent = NULL);
    ~StatusThread();

    void run();

public slots:
    void set_current_printer(const QString& printer);
    bool is_locked_get_status();

public:
    QList<Printer_struct> printers;
    QStringList printerlist;

    bool abort;

private:
    bool locker_get_status;
    DeviceManager* devicemanager;
    CupsManager cupsmanager;
    StatusManager statusmanager;

    QString current_printer;
    QMutex mutex;
    PRINTER_STATUS status;
};

#endif // STATUSTHREAD_H
