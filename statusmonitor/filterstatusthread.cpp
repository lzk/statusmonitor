#include "filterstatusthread.h"
#include "jkinterface.h"
#include "filtertask.h"
#include "trans.h"
#include "smconfig.h"
FilterStatusThread::FilterStatusThread(QObject *parent)
    : QThread(parent)
{
    abort = false;
//    statusmanager.clearFile();
}

FilterStatusThread::~FilterStatusThread()
{
    abort = true;
    wait();
}

void FilterStatusThread::run()
{
//    PRINTER_STATUS printer_status;
    int result;

    char data[1025];
    int datalen = 1024;

    Trans_Client tc(g_status_server_path);
    char buffer[1124];

//    QString str;
    forever {
        if (abort){
            usleep(100 * 1000);
            return;
        }

        result = cups_usb_getDeviceID(data ,datalen);


        sprintf(buffer ,"dvid://%s?deviceid=%s" ,current_printer.toLatin1().constData() ,data);
        tc.writeThenRead(buffer ,1124);

//        str = QString("echo %1 >> /tmp/abcd.log").arg(data);
//        system(str.toLatin1().constData());

        sleep(6);
    }
}

void FilterStatusThread::set_current_printer(const QString& printer)
{
    QMutexLocker locker(&mutex);
    current_printer = printer;
}
#ifdef __cplusplus
extern "C" {
#endif

static FilterStatusThread* filter_status_thread = NULL;
int filter_task_start(const  char* printer_name ,const char* printer_uri)
//int filter_task_start(filter_task_struct* filter_data)
{
    if(!QString(printer_uri).startsWith("usb://")){
        LOGLOG("filter_status_task: only usb uri can run the thread!");
        return -1;
    }
    if(filter_status_thread){
        LOGLOG("filter_status_task: the thread had been running!");
        return -2;
    }
    filter_status_thread = new FilterStatusThread;
    filter_status_thread->set_current_printer(printer_name);
    filter_status_thread->start();
    return 0;
}

int filter_task_end()
{
    if(filter_status_thread){
        delete filter_status_thread;
        filter_status_thread = NULL;
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
