#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
class StatusThread;

class Watcher : public QThread
{
    Q_OBJECT
public:
    explicit Watcher(QObject *parent = 0);
    ~Watcher();
    void run();
    void timerOut();

signals:
    void server_restart();

private:
    bool abort;
};

#endif // WATCHER_H
