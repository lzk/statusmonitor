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
    void set_device_id(const QString& printer ,const QString& device_id);

public:
    QList<Printer_struct> printers;
    QStringList printerlist;

    bool abort;

private:
    DeviceManager* devicemanager;
    CupsManager cupsmanager;
    StatusManager statusmanager;

    QString current_printer;
    QMutex mutex;
    PRINTER_STATUS status;
};

#endif // STATUSTHREAD_H
