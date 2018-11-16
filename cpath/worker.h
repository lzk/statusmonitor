#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include "statusmonitor.h"
#include "serverthread.h"
#include "devicemanager.h"

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
    void cmdFromUi(int cmd ,QVariant data = QVariant());
    void getPrinters();


private:
    int cmd_status;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;
//    QList<Job_struct> jobs;
    StatusMonitor m_statusMonitor;
    DeviceManager deviceManager;
    DeviceIO* device;
};

#endif // WORKER_H
