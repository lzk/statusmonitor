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
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    void setPrinters(PrinterInfo_struct* ps);
    DeviceIO* getDevice(const char* device_uri);
    
signals:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    
public slots:
    void cmdFromUi(int cmd ,const QString& printer_name = QString() ,QVariant data = QVariant());
    void getPrinters();
    void set_cancel(bool);

private:
    int cmd_status;
    QStringList printers;
    DeviceIO* device;
    LShell* lshell;
    ScannerApp* scanner;
    QList<PrinterInfo_struct> printers_detail;
    StatusMonitor m_statusMonitor;
    DeviceManager deviceManager;

    Printer_struct* get_printer(const QString& printer_name);//get printer exist in system
};

#endif // WORKER_H
