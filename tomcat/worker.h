#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include "statusmonitor.h"
#include "serverthread.h"
#include "watcher.h"
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    
signals:
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    void set_current_printer(const QString& printer);
    
public slots:
    void cmdFromUi(int cmd ,const QString& printer_name = QString() ,QVariant data = QVariant());
//    void update_printer_status(const QString&);
    void update_current_printer_status();
    void update_printerlist();

private:
    int cmd_status;
    DeviceIO* device;
    Watcher* watcher;
};

#endif // WORKER_H
