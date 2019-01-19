#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
#include <QTimer>
#include "statusmonitor.h"
#include <QMutex>
class StatusThread;

class Watcher : public QThread
{
    Q_OBJECT
public:
    explicit Watcher(QObject *parent = nullptr);
    ~Watcher();
    void run();

    int get_printer_info(const QString& printer_name ,PrinterInfo_struct&);
    void setPrinters(PrinterInfo_struct* ps);

signals:
    void update_printer_status(PrinterStatus_struct);
    void update_printerlist(QList<PrinterInfo_struct>);

public slots:
    void timerOut();
    void set_current_printer(const QString& printer);

private:
    void getPrinters();

private:
    QTimer timer;
    QMutex mutex;
private:
    StatusThread* statusThread;
    QStringList printers;
    QList<PrinterInfo_struct> printers_detail;

    QString current_printer;
    QStringList current_printers;
    QList<PrinterInfo_struct> current_printers_detail;
    PrinterStatus_struct current_printer_status;
};

#endif // WATCHER_H
