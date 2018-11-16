#include "serverthread.h"
#include <unistd.h>
#include "jkinterface.h"
ServerThread::ServerThread(const char* server_path ,QObject *parent)
    : QThread(parent)
{
    m_trans_back = 0;
    abort = false;
    m_result = "ok";
    trans_server.createServer(server_path);
    start();
}

ServerThread::~ServerThread()
{
    abort = true;
    wait();
}

int callback_Server(void* para,char* buffer,int bufsize)
{
    ServerThread* st = (ServerThread*)para;
    QString cmd(buffer);
    if(!cmd.compare("check")){
        LOGLOG("filter check");
        st->send_cmd(cmd);
        int timeout = 3000;
        while (!st->trans_back() && timeout --) {
            usleep(10000);
        }
        memset(buffer ,0 ,bufsize);
        strcpy(buffer ,st->result().toLatin1().constData());

        switch (st->trans_back()) {
        case 0:
            if(timeout)
                st->send_cmd("timeout");
            break;
        case 1:
            st->send_cmd("checked");
            break;
        case 2:
//            st->send_cmd("cancel");//no need close dialog again.
            break;
        default:
            break;
        }
    }
    return 0;
}

void ServerThread::run()
{
    forever {
        if (abort)
            return;
        trans_server.readThenWrite(callback_Server ,this);
    }
}

void ServerThread::cmd_result(const QString &result)
{
    m_result = result;
    if(!result.compare("ok")){
        m_trans_back = 1;
    }else if(!result.compare("cancel")){
        m_trans_back = 2;
    }
}

void ServerThread::send_cmd(const QString& cmd)
{
    m_trans_back = false;
    client_cmd(cmd);
}
