#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include "statusmonitor.h"
#include "serverthread.h"
#include "devicemanager.h"
#ifdef TOMCAT
#include "tomcat.h"
#endif

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    void setPrinters(PrinterInfo_struct* ps);
#ifdef TOMCAT
    void setJobs(const char* str);
#endif
    DeviceIO* getDevice(const char* device_uri);
    
signals:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    
public slots:
    void cmdFromUi(int cmd ,QVariant data = QVariant());
    void getPrinters();

#ifdef TOMCAT
    void getJobs();
#endif

private:
    int cmd_status;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;
//    QList<Job_struct> jobs;
    StatusMonitor m_statusMonitor;
    DeviceManager deviceManager;
    DeviceIO* device;
#ifdef TOMCAT
    QStringList jobs;
    Tomcat m_tomcat;
#endif
};

#endif // WORKER_H
