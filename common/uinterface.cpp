#include "uinterface.h"
#include "worker.h"

#include "DataStruct.h"
UInterface::UInterface(QObject *parent) :
    QObject(parent)
{
    worker = new Worker();
    worker->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,worker ,SLOT(deleteLater()));

    connect(this ,SIGNAL(cmdToWorker(int,QVariant)) ,worker ,SLOT(cmdFromUi(int,QVariant)));
    connect(worker ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SIGNAL(cmdResult(int,int,QVariant)));
    connect(worker ,SIGNAL(client_cmd(QString)) ,this ,SIGNAL(client_cmd(QString)));

    thread.start();

    connect(&timer ,SIGNAL(timeout()) ,this ,SLOT(timerOut()));

}

UInterface::~UInterface()
{

    thread.quit();
    thread.wait();
}

void UInterface::setCmd(int cmd ,QVariant data)
{
    cmdToWorker(cmd ,data);
}

void UInterface::setTimer(int timeVal)
//void UInterface::setTimer(int timeVal ,const QObject *receiver ,const char* slot)
{
//    timer.disconnect(SIGNAL(timeout()));
//    connect(&timer ,SIGNAL(timeout()) ,receiver ,slot);
    if(!timeVal)
        timer.stop();
    else{
        timer.start(timeVal * 1000);
    }
}

void UInterface::timerOut()
{
    cmdToWorker(DataStruct::CMD_GetStatus ,current_printer);
}

