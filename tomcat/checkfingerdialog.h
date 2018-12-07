#ifndef CHECKFINGERDIALOG_H
#define CHECKFINGERDIALOG_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class CheckFingerDialog;
}

class CheckFingerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CheckFingerDialog(const QString& job_info ,QWidget *parent = 0);
    ~CheckFingerDialog();

signals:
    void cancel_job(int jobid);
    void job_timeout(int jobid);

private slots:
    void timeout();
    void cancel_and_delete();
    void active_window(int jobid);

public slots:
    void start_check_finger(int jobid);

private:
    Ui::CheckFingerDialog *ui;
    QMovie *movie;
    int jobid;
    int time_val;
    QString job_info;
    QString printer_name;
    QTimer timer;
};

#endif // CHECKFINGERDIALOG_H
