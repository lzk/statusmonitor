#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include "jkinterface.h"

class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(const char* server_path ,QObject *parent = NULL);
    ~ServerThread();

    void run();
    void send_cmd(const QString&);
    const QString& result(){return m_result;}
    int trans_back(){return m_trans_back;}
signals:
    void client_cmd(const QString &s);
public slots:
    void cmd_result(const QString &s);

private:
    Trans_Server trans_server;
    QString m_result;
    int m_trans_back;
    bool abort;
};

#endif // SERVERTHREAD_H
