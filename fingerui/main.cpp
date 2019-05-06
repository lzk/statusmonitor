#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <signal.h>
#include <unistd.h>
#include <QSettings>
#include <QMessageBox>
#include "../toec/filterlib.h"
int g_jobid = 0;
MainWindow* ww;

void quit(int);
void result_quit(int);
void handler_result(int result);
void update_result(int jobid ,int result)
{
    QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
    QString value = QString("%1,%2").arg(result).arg(getpid());
    settings.setValue(QString("%1").arg(jobid) ,value);

    handler_result(result);
    if(result != Checked_Result_checking)
        quit(0);
}

QString get_result(int jobid)
{
    QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
    QString str_jobid = QString("%1").arg(jobid);
    QString value = settings.value(str_jobid).toString();
    return value;
}

void handler_result(int result)
{
    int finger_checked_result;
    finger_checked_result = result;

    if(finger_checked_result != Checked_Result_Disable){

        if(finger_checked_result == Checked_Result_DevBusy)
        {
            QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("设备忙，当前打印作业将被取消！"));
            msg.setStandardButtons (QMessageBox::Ok);
            msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
            msg.exec ();
        }
        else if(finger_checked_result == Checked_Result_Fail)
        {
            QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("指纹验证失败，打印作业将被取消！"));
            msg.setStandardButtons (QMessageBox::Ok);
            msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
            msg.exec ();
        }
        else if(finger_checked_result == Checked_Result_NoFinger)
        {
            QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("设备用户列表为空，无法进行指纹验证，当前打印作业将被取消！"));
            msg.setStandardButtons (QMessageBox::Ok);
            msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
            msg.exec ();
        }
//        else if(finger_checked_result == Checked_Result_Abort_Print)
//        {
//            QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("当前打印作业被取消！"));
//            msg.setStandardButtons (QMessageBox::Ok);
//            msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
//            msg.exec ();
//        }
        else if(finger_checked_result == Checked_Result_timeout)
        {
            QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("指纹验证超时，打印作业也将被取消！"));
            msg.setStandardButtons (QMessageBox::Ok);
            msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
            msg.exec ();
        }
        else if(finger_checked_result == Checked_Result_Cancel|| finger_checked_result == Checked_Result_invalidJobid)
        {
            QMessageBox msg(QMessageBox::Warning,QString::fromUtf8("警告"),QString::fromUtf8("指纹验证被用户取消，打印作业也将被取消！"));
            msg.setStandardButtons (QMessageBox::Ok);
            msg.setButtonText (QMessageBox::Ok,QString::fromUtf8("确定"));
            msg.exec ();
        }
    }
}

void quit(int)
{
    if(qApp)
        qApp->quit();
}

void result_quit(int signal)
{
#if 0
    int result = Checked_Result_invalidJobid;
    int jobid = g_jobid;

    QString value = get_result(jobid);
    if(!value.isEmpty()){
        QStringList columns = value.split(",");
        result = columns.at(0).toInt();
    }
//    update_result(jobid ,result);
#else
    int result = signal - 34;
#endif
    ww->hide();
    handler_result(result);
    quit(0);

}

int main(int argc, char *argv[])
{
    signal(SIGINT ,quit);
    for(int i = 0 ;i < Checked_Result_max ;i ++){
        signal(34 + i ,result_quit);
    }

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/image/app_icon.png"));
    a.setQuitOnLastWindowClosed(false);

//    QCoreApplication::setOrganizationName("TOEC");
////    QCoreApplication::setOrganizationDomain("mysoft.com");
//    QCoreApplication::setApplicationName("FingerUi");
#if QT_VERSION < 0x050000
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif

    QStringList arguments = QCoreApplication::arguments();
    QString str;
    if(arguments.count() < 2)
        str = "start://testprinter?jobid=0&time_val=30";
    else
        str = arguments[1];
    MainWindow w(str);
    ww = &w;
    w.show();

    a.exec();
    return 0;
}
