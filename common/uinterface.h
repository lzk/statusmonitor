#ifndef UINTERFACE_H
#define UINTERFACE_H

#include <QObject>
#include <QThread>
#include <QVariant>
#include <QTimer>
#include "DataStruct.h"
class Worker;
class UInterface : public QObject
{
    Q_OBJECT
public:
    explicit UInterface(QObject *parent = 0);
    ~UInterface();
    void setCmd(int cmd ,QVariant data = QVariant());
    void setTimer(int timeVal = 0);
    void setcurrentPrinter(const QString& str){current_printer = str;}
    
signals:
    void cmdToWorker(int cmd ,QVariant data = QVariant());
    void cmdResult(int cmd,int result ,QVariant data=QVariant());
    void client_cmd(const QString &s);

private slots:
    void timerOut();

private:
    Worker* worker;

    QThread thread;
    QTimer timer;

    QString current_printer;
};

extern UInterface* gUInterface;
#endif // UINTERFACE_H
