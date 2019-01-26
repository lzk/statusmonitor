#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
#include <QTimer>
#include "statusmonitor.h"
#include "devicemanager.h"
#include <QMutex>
#include <QStringList>
class StatusThread;

class Watcher : public QThread
{
    Q_OBJECT
public:
    explicit Watcher(DeviceManager* device_manager ,QObject *parent = 0);
    ~Watcher();
    void run();
    void timerOut();

    int get_printer_list(QList<PrinterInfo_struct>&);
    int get_printer_info(const QString& printer_name ,PrinterInfo_struct&);
    void setPrinters(PrinterInfo_struct* ps);

signals:
    void update_printer_status(PrinterInfo_struct);
    void update_printerlist();

public slots:
    void set_current_printer(const QString& printer);

private:
    void getPrinters();

private:
    bool abort;
    QMutex mutex;
private:
    DeviceManager* device_manager;
    StatusThread* statusThread;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;

    QString current_printer;
    QStringList current_printers;
    QList<PrinterInfo_struct> current_printers_detail;
    PrinterStatus_struct current_printer_status;
};

#endif // WATCHER_H
