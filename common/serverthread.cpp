#include "serverthread.h"
#include <QFile>
ServerThread::ServerThread(const char* server_path ,QObject *parent)
    : QThread(parent)
{
    abort = false;
    trans_server.createServer(server_path);
    LOGLOG("create server:%s" ,server_path);
}

ServerThread::~ServerThread()
{
    abort = true;
    const char* server_path = trans_server.get_server_path();
    LOGLOG("delete server:%s" ,server_path);
    Trans_Client tc(server_path);
    tc.tryConnectToServer();
//    while(abort)
//        ;
    if(QFile::exists(server_path))
        QFile::remove(server_path);
    wait(100);
}

void ServerThread::run()
{
    forever {
        if (abort)
            break;
        int fd = trans_server.any_client_connected();
        if(fd >= 0){
            client_connect(fd);
        }
    }
    abort = false;
//    exec();
}

