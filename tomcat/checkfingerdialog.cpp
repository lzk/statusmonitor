#include "checkfingerdialog.h"
#include "ui_checkfingerdialog.h"
#include <QMovie>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QUrl>

#if QT_VERSION > 0x050000
#include <QUrlQuery>
#endif
#include "filterlib.h"
#include "jkinterface.h"
#include <QSettings>
#include <QDir>
CheckFingerDialog::CheckFingerDialog(const QString& _job_info ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckFingerDialog),
    time_val(30),
    job_info(_job_info)
{
    ui->setupUi(this);

    ui->buttonBox->removeButton(ui->buttonBox->button(QDialogButtonBox::Ok));

    ui->label_gif->setPixmap(QPixmap(":/image/finger.gif"));
//    movie = new QMovie(":/image/finger.gif");
//    ui->label_gif->setMovie(movie);

    QString homepath = QDir::homePath();
    QSettings setting(homepath + "/.tjgd1z/setting.ini" ,QSettings::defaultFormat());
    bool ok;
    time_val = setting.value("TimeOut").toInt(&ok);
    if(!ok)
        time_val = 30;
    ui->label_timeval->setText(QString("%1").arg(time_val));

    timer.setInterval(1000);
    connect(&timer ,SIGNAL(timeout()) ,this ,SLOT(timeout()));

    connect(this ,SIGNAL(rejected()) ,this ,SLOT(cancel_and_delete()));

    QUrl url(job_info);
//    QString cmd = url.scheme();
    printer_name = url.host();

#if QT_VERSION > 0x050000
    jobid = QUrlQuery(QUrl(url)).queryItemValue("jobid").toInt();
#else
    jobid = QUrl(url).queryItemValue("jobid").toInt();
#endif
//    setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QString("指纹认证-") + printer_name + QString("-%1").arg(jobid));
    start_check_finger(jobid);
}

CheckFingerDialog::~CheckFingerDialog()
{
    delete ui;
}

void CheckFingerDialog::cancel_and_delete()
{
    cancel_job(jobid);
//    delete this;
}

void CheckFingerDialog::timeout()
{
//    raise();
    time_val --;
    ui->label_timeval->setText(QString("%1").arg(time_val));
    if(!time_val){
        job_timeout(jobid);
//        delete this;
    }
}

void CheckFingerDialog::start_check_finger(int jobid)
{
    if(jobid != this->jobid)
        return;
    timer.start();
    movie = new QMovie(":/image/finger.gif");
    ui->label_gif->setMovie(movie);
    movie->start();
//    raise();
}

void CheckFingerDialog::active_window(int jobid)
{
    if(jobid != this->jobid){
//        lower();
//        LOGLOG("lower jobid:%d" ,this->jobid);
    }else{
        LOGLOG("raise jobid:%d" ,jobid);
        show();
        raise();
    }
}
