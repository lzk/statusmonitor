#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include "statusmonitor.h"
#include "serverthread.h"
#include "devicemanager.h"
#include "lshell.h"
#include "scannerapp.h"
#include "watcher.h"
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    void update_scan_progress(Printer_struct* printer ,int progress);
    
signals:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    void signal_update_scan_progress(int progress);//(0 - 100)
    void set_current_printer(const QString& printer);
    
public slots:
    void cmdFromUi(int cmd ,const QString& printer_name = QString() ,QVariant data = QVariant());
    void cancel();
    void update_printer_status(PrinterInfo_struct);
    void update_printerlist();

private:
    int cmd_status;
    QStringList printers;
    DeviceManager* deviceManager;
    LShell* lshell;
    ScannerApp* scanner;
    QList<PrinterInfo_struct> printers_detail;
    PrinterInfo_struct current_printer_info;
    Watcher* watcher;

    Printer_struct* get_printer(const QString& printer_name);//get printer exist in system
    bool cmd_status_validate(Printer_struct* printer ,int cmd);
};

#endif // WORKER_H
