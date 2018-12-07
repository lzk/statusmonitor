#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include "jkinterface.h"

class ClientThread;
class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(const char* server_path ,QObject *parent = NULL);
    ~ServerThread();

    void run();
signals:
    void client_connect(int fd);
    void client_cmd(const QString &s ,void* para);

private:
    Trans_Server trans_server;
    bool abort;

    friend class ClientThread;
};

#endif // SERVERTHREAD_H
