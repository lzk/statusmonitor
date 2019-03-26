#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
#include "statusmonitor.h"
#include "devicemanager.h"
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
    void timerOut();

    int get_printer_list(QList<PrinterInfo_struct>&);
    int get_printer_info(const QString& printer_name ,PrinterInfo_struct&);
    void setPrinters(PrinterInfo_struct* ps);

signals:
    void update_printer_status(PrinterInfo_struct);
    void update_printerlist();
    void server_restart();

public slots:
    void set_current_printer(const QString& printer);

private:
    void getPrinters();
    int printerlist_compare(QList<PrinterInfo_struct> & ,QList<PrinterInfo_struct> &);

private:
    DeviceManager* device_manager;
    StatusThread* statusThread;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;

    QString current_printer;
    QStringList current_printers;
    QList<PrinterInfo_struct> current_printers_detail;
    PrinterStatus_struct current_printer_status;

private:
    bool abort;
    QMutex mutex;
};

#endif // WATCHER_H
