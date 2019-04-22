#ifndef APPSERVER_H
#define APPSERVER_H

#include <QObject>
#include <QThread>
class ServerThread;
class StatusThread;
class AppServer : public QObject
{
    Q_OBJECT
public:
    explicit AppServer(const char* server_path ,QObject *parent = 0);
    ~AppServer();
    void delete_finger_dialog(int id);

public slots:
    void client_cmd(const QString &s ,void* para);
    void client_connect(int fd);
    void restart_server();


private:
    ServerThread* thread_server;
    QString server_path;


};

#endif // APPSERVER_H
