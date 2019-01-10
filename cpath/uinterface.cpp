#include "uinterface.h"
#include "worker.h"
UInterface::UInterface(QObject *parent) :
    QObject(parent)
  ,timeval(0)
{
    worker = new Worker();
    worker->moveToThread(&thread);
    connect(&thread ,SIGNAL(finished()) ,worker ,SLOT(deleteLater()));

    connect(this ,SIGNAL(cmdToWorker(int,QString ,QVariant)) ,worker ,SLOT(cmdFromUi(int,QString ,QVariant)));
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

void UInterface::setCmd(int cmd ,const QString& printer_name ,QVariant data)
{
    cmdToWorker(cmd ,printer_name ,data);
}

void UInterface::setCurrentPrinterCmd(int cmd ,QVariant data)
{
    setCmd(cmd ,current_printer ,data);
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
    //cmdToWorker(UIConfig::CMD_GetPrinters);
    cmdToWorker(UIConfig::CMD_GetStatus ,current_printer);
}

void UInterface::cmdResult_slot(int cmd,int result ,QVariant data)
{
    (void)result;
    (void)data;
    switch (cmd) {
    case UIConfig::CMD_GetStatus:
        qDebug("UInterface");
        if(timeval > 0){
            timer.start(timeval * 1000);
        }
//        if(!result){
//            emit updateStatus(data);
//        }else{//get status fail
//            LOGLOG("get printer status fail!");
//        }
        break;
    default:
        break;
    }
}

void UInterface::setcurrentPrinter(const QString& str)
{
    current_printer = str;
}

void UInterface::setDeviceMsgFrmUI(const QString &str, int result)
{
    emit setDeviceMsg(str,result);
}

