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

void update_status(int jobid ,int status)
{
    QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
    settings.beginGroup(QString("%1").arg(jobid));
    settings.setValue("status" ,QString("%1").arg(status));
    settings.endGroup();
    settings.sync();
}
void update_result(int jobid ,int result)
{
    QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
    settings.beginGroup(QString("%1").arg(jobid));
    settings.setValue("result" ,QString("%1").arg(result));
    settings.endGroup();
    settings.sync();
}

int get_result(int jobid)
{
    bool ok;
    QSettings settings("/tmp/.tjgd1zsmtmp.conf" ,QSettings::NativeFormat);
    settings.beginGroup(QString("%1").arg(jobid));
    int value = settings.value("result").toInt(&ok);
    settings.endGroup();
    if(!ok)
        value = Checked_Result_checking;
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

void timeout_task(int jobid)
{
    int result = get_result(jobid);
    if(result == Checked_Result_checking){

    }else{
        if(!ww->isHidden())
            ww->hide();
        handler_result(result);
        quit(0);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT ,quit);

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
