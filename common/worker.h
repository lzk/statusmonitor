#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include "StatusMonitor.h"
#include "serverthread.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    void setPrinters(PrinterInfo_struct* ps);
    void setJobs(const char* str);
    
signals:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());

    void client_cmd(const QString &s);
    
public slots:
    void cmdFromUi(int cmd ,QVariant data = QVariant());
    void getPrinters();
    void getJobs();

private:
    int cmd_status;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;
//    QList<Job_struct> jobs;
    QStringList jobs;
    StatusMonitor m_statusMonitor;

    ServerThread thread_server;
};

#endif // WORKER_H
