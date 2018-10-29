#include "serverthread.h"
#include "log.h"

#include <unistd.h>
ServerThread::ServerThread(QObject *parent)
    : QThread(parent)
{
    m_trans_back = false;
    abort = false;
    m_result = "ok";
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
        if(timeout)
            st->send_cmd("checked");
        else
            st->send_cmd("timeout");
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
    if(!result.compare("ok")){
        m_trans_back = true;
    }
}

void ServerThread::send_cmd(const QString& cmd)
{
    m_trans_back = false;
    client_cmd(cmd);
}
