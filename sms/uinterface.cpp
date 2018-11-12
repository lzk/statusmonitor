#include "uinterface.h"
#include "worker.h"
#include "uiconfig.h"
UInterface::UInterface(QObject *parent) :
    QObject(parent)
  ,timeval(0)
{
    worker = new Worker();
    worker->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,worker ,SLOT(deleteLater()));

    connect(this ,SIGNAL(cmdToWorker(int,QVariant)) ,worker ,SLOT(cmdFromUi(int,QVariant)));
    connect(worker ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SIGNAL(cmdResult(int,int,QVariant)));
    connect(worker ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult_slot(int,int,QVariant)));

    thread.start();

    connect(&timer ,SIGNAL(timeout()) ,this ,SLOT(timerOut()));
    timer.setSingleShot(true);

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
{
    this->timeval = timeVal;
    if(timeVal < 1)
        timer.stop();
    else{
        timer.start(timeVal * 1000);
    }
}

void UInterface::timerOut()
{
    cmdToWorker(UIConfig::CMD_GetStatus ,current_printer);
}

void UInterface::cmdResult_slot(int cmd,int result ,QVariant data)
{
    (void)result;
    (void)data;
    switch (cmd) {
    case UIConfig::CMD_GetStatus:
        if(timeval > 0){
            timer.start(timeval * 1000);
        }
        break;
    default:
        break;
    }
}
