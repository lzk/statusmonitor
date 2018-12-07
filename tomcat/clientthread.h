#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "serverthread.h"
class FingerHandler;
class ClientThread :public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(ServerThread* _server ,int _fd ,QObject *parent = NULL)
        :QThread(parent)
          ,server(_server)
    ,fd(_fd){

    }
    ~ClientThread(){
//        quit();
        wait();
    }
    void run();
    const QString& result(){return m_result;}
    int trans_back(){return m_trans_back;}
    void send_cmd(const QString& cmd);
    void check_finger(int jobid);

signals:
    void signal_check_finger(int jobid);
    void client_cmd(const QString &s ,void* para);
    void delete_dialog();

public slots:
    void check_finger_result(int,int);
    void cancel(int jobid);
    void timeout(int jobid);
    void client_cmd_result(const QString &s);

public:
    ServerThread* server;
    int fd;
    QString m_result;
    int m_trans_back;
    int jobid;

    bool m_cancel;
    bool m_timeout;

    friend class FingerHandler;
};
#endif // CLIENTTHREAD_H
