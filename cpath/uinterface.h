#ifndef UINTERFACE_H
#define UINTERFACE_H

#include <QObject>
#include <QThread>
#include <QVariant>
#include <QTimer>
#include "uiconfig.h"
class Worker;
class UInterface : public QObject
{
    Q_OBJECT
public:
    explicit UInterface(QObject *parent = 0);
    ~UInterface();
    void setCmd(int cmd ,const QString& printer_name ,QVariant data = QVariant());
    void setTimer(int timeVal = 0);
    void setcurrentPrinter(const QString& str);//{current_printer = str;}
    void setCurrentPrinterCmd(int cmd ,QVariant data = QVariant());
    void setDeviceMsgFrmUI(const QString& str, int result);
    
signals:
    void cmdToWorker(int cmd ,const QString& printer_name = QString() ,QVariant data = QVariant());
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    void setDeviceMsg(const QString& str, int result);

private slots:
    void timerOut();
    void cmdResult_slot(int cmd,int result ,QVariant data=QVariant());

private:
    Worker* worker;

    QThread thread;
    QTimer timer;
    int timeval;

    QString current_printer;
};

extern UInterface* gUInterface;
#endif // UINTERFACE_H