#ifndef APPSERVER_H
#define APPSERVER_H

#include <QObject>
#include <QThread>
class ServerThread;
class AppServer : public QObject
{
    Q_OBJECT
public:
    explicit AppServer(const char* server_path ,QObject *parent = 0);
    ~AppServer();

public slots:
    void client_cmd(const QString &s ,void* para);
    void client_connect(int fd);

private:
    ServerThread* thread_server;
    QThread thread;

//    Trans_Server trans_server;

};

#endif // APPSERVER_H
