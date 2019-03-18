#include "serverthread.h"

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
    Trans_Client tc(trans_server.get_server_path());
    tc.tryConnectToServer();
    while(abort)usleep(1000);
}

void ServerThread::run()
{
    forever {
        if (abort){
            break;
        }
        int fd = trans_server.any_client_connected();
        if(fd >= 0){
            client_connect(fd);
        }
    }
    abort = false;
}

