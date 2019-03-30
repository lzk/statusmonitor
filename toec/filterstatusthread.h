#ifndef FILTERSTATUSTHREAD_H
#define FILTERSTATUSTHREAD_H

#include <QThread>
#include <QMutex>
class FilterStatusThread : public QThread
{
    Q_OBJECT
public:
    explicit FilterStatusThread(QObject *parent = NULL);
    ~FilterStatusThread();
    void set_current_printer(const QString& printer);

    void run();

private:
    bool abort;
    QString current_printer;
    QMutex mutex;
};

#endif // FILTERSTATUSTHREAD_H
