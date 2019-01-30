#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include "statusmonitor.h"
#include "serverthread.h"
#include "devicemanager.h"
#include "watcher.h"
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    void setPrinters(PrinterInfo_struct* ps);
    
signals:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    void set_current_printer(const QString& printer);
    
public slots:
    void cmdFromUi(int cmd ,const QString& printer_name = QString() ,QVariant data = QVariant());
    void getPrinters();
    void update_printer_status(PrinterInfo_struct);
    void update_printerlist();

private:
    int cmd_status;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;
    DeviceManager* deviceManager;
    DeviceIO* device;
    Printer_struct* get_printer(const QString& printer_name);//get printer exist in system

    Watcher* watcher;
};

#endif // WORKER_H