#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUrl>
#include <QMovie>

#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
#include "../toec/filterlib.h"

extern int g_jobid;
void update_result(int jobid ,int result);
MainWindow::MainWindow(const QString& _job_info ,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
  job_info(_job_info)
{
    ui->setupUi(this);

    ui->label_gif->setPixmap(QPixmap(":/image/finger.gif"));
//    movie = new QMovie(":/image/finger.gif");
//    ui->label_gif->setMovie(movie);


    QUrl url(job_info);
//    QString cmd = url.scheme();
    printer_name = url.host();

    QString str = job_info;
    int index;
    index = str.indexOf("://");
//        cmd = str.left(index);
//        LOGLOG("cmd is:%s" ,cmd.toLatin1().constData());
    QString printer = job_info.mid(index + strlen("://"));
    index = printer.indexOf('?');
    if(index > 0)
        printer = printer.left(index);
    printer_name = printer;

#if QT_VERSION > 0x050000
    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
    time_val = QUrlQuery(QUrl(url)).queryItemValue("time_val").toInt();
#else
    jobid = QUrl(url).queryItemValue("jobid").toInt();
    time_val = QUrl(url).queryItemValue("time_val").toInt();
#endif
    g_jobid = jobid;
    if(time_val < 10)
        time_val = 30;

    update_result(g_jobid ,Checked_Result_checking);
//    setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QString("指纹认证-") + printer_name + QString("-%1").arg(jobid));

    ui->label_timeval->setText(QString("%1").arg(time_val));
    ui->label->setText(QString("您的打印工作需要进行指纹认证，请按下您的手指。如果在%1秒内未检测到正确指纹，则打印工作取消。您也可以直接点击“取消”按键取消当前打印。")
                       .arg(time_val));

    connect(&timer ,SIGNAL(timeout()) ,this ,SLOT(timeout()));

    timer.setInterval(1000);
    timer.start();
    movie = new QMovie(":/image/finger.gif");
    ui->label_gif->setMovie(movie);
    movie->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeout()
{
    if(time_val){
        time_val --;
        ui->label_timeval->setText(QString("%1").arg(time_val));
    }else{
        hide();
        update_result(jobid ,Checked_Result_timeout);
        close();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    update_result(jobid ,Checked_Result_Cancel);
    QMainWindow::closeEvent(event);
}
