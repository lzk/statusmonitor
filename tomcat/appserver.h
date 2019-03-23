#ifndef APPSERVER_H
#define APPSERVER_H

#include <QObject>
#include <QThread>
//class FingerHandler;
class ServerThread;
class CheckFingerDialog;
typedef
struct{
    CheckFingerDialog* dialog;
    int result;
    int id;
}
    FingerResult_struct;
class AppServer : public QObject
{
    Q_OBJECT
public:
    explicit AppServer(const char* server_path ,QObject *parent = 0);
    ~AppServer();

    void new_finger_dialog(int id ,const QString&);
    void delete_finger_dialog(int id);
    int get_finger_result(int id);

    void set_device_id(const QString& printer ,const QString& device_id);
public slots:
    void client_cmd(const QString &s ,void* para);
    void client_connect(int fd);

    void cancel(int jobid);
    void timeout(int jobid);
    void restart_server();

signals:
    void signal_set_device_id(const QString& printer ,const QString& device_id);

private:
//    FingerHandler* finger_handler;
    ServerThread* thread_server;
    QString server_path;
//    QThread thread;

//    Trans_Server trans_server;

    QList<FingerResult_struct > finger_result_list;
};

#endif // APPSERVER_H
