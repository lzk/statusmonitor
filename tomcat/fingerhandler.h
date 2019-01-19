#ifndef FINGERHANDLER_H
#define FINGERHANDLER_H

#include <QObject>
class ClientThread;
class FingerHandler : public QObject
{
    Q_OBJECT
public:
    explicit FingerHandler(ClientThread *_cth);
//    void active_current_jobid_window();
    int is_finger_enable();
    int check_finger(const char* uri ,int id);

signals:
//    void check_finger_result(int jobid ,int result);
//    void start_check_finger(int jobid);
//    void active_window(int jobid);

public slots:
//    void check_finger(const QString& cmd);
    void cancel();

private:
//    int jobid;
    ClientThread* cth;
};

#endif // FINGERHANDLER_H
