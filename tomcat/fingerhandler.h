#ifndef FINGERHANDLER_H
#define FINGERHANDLER_H

#include <QObject>
class FingerHandler : public QObject
{
    Q_OBJECT
public:
    explicit FingerHandler(QObject *parent = 0);
    void active_current_jobid_window();

signals:
    void check_finger_result(int jobid ,int result);
    void start_check_finger(int jobid);
    void active_window(int jobid);

public slots:
    void check_finger(int jobid);

private:
    int jobid;
};

#endif // FINGERHANDLER_H
