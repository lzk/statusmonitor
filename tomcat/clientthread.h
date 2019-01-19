#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "serverthread.h"
class FingerHandler;
class ClientThread :public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(ServerThread* _server ,int _fd ,QObject *parent = NULL);
    void run();
    const QString& result(){return m_result;}
    int trans_back(){return m_trans_back;}
    void send_cmd(const QString& cmd);
    void check_finger(const QString& cmd);

signals:
    void signal_check_finger(const QString& cmd);
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
    bool print_check_flag;
    int finger_checked_result;

    bool m_cancel;
    bool m_timeout;
    FingerHandler* finger_handler;

    friend class FingerHandler;

};
#endif // CLIENTTHREAD_H
