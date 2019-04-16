#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QCloseEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& job_info ,QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timeout();

protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;
    QMovie *movie;
    int jobid;
    int time_val;
    QString job_info;
    QString printer_name;
    QTimer timer;
};

#endif // MAINWINDOW_H
