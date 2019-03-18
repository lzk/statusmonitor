#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
#include "statusmonitor.h"
#include "devicemanager.h"
#include "statusmanager.h"
#include <QMutex>
#include <QStringList>
class StatusThread;

class Watcher : public QThread
{
    Q_OBJECT
public:
    explicit Watcher(QObject *parent = 0);
    ~Watcher();
    void run();

    int get_printer_list(QList<PrinterInfo_struct>&);
    int get_printer_info(const QString& printer_name ,PrinterInfo_struct&);
    void setPrinters(PrinterInfo_struct* ps);
    void setPrinters(Printer_struct* ps);
    bool isabort(){return abort;}

signals:
    void update_printer_status(PrinterInfo_struct);
    void update_printerlist();

public slots:
    void set_current_printer(const QString& printer);

protected:
    void watcher_job();

private:
    void getPrinters();
    int printerlist_compare(QList<PrinterInfo_struct> & ,QList<PrinterInfo_struct> &);

private:
//    DeviceManager* device_manager;
    StatusThread* statusThread;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;

    QString current_printer;
    QStringList current_printers;
    QList<PrinterInfo_struct> current_printers_detail;
    PrinterStatus_struct current_printer_status;
    StatusManager statusmanager;

private:
    bool abort;
    QMutex mutex;

    friend class StatusThread;
};

#endif // WATCHER_H
