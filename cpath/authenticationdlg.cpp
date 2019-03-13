#include "authenticationdlg.h"
#include "ui_authenticationdlg.h"
#include "uinterface.h"
#include "qdebug.h"
#define RETRYTIMES 3

AuthenticationDlg::AuthenticationDlg(QWidget *parent, bool*confirm) :
    QDialog(parent),
    ui(new Ui::AuthenticationDlg)
{
    ui->setupUi(this);

    isconfirm = confirm;
    QRegExp rx("^[\x0020-\x007e]{1,8}$");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    ui->lineEdit_Password->setValidator(validator);
    ui->lineEdit_Password->setEnabled(true);
    ui->lineEdit_Password->clear();
    ui->lineEdit_Password->setEnabled(true);
    //ui->label_msg->setText(tr("The default password for this unit is \"888888\".\
   //                            For security of your equipment, please change this password."));
    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

    ui->cycleWidget->hide();
    cycle = new BusyRefreshLabel(ui->cycleWidget,true);
    cycle->setGeometry(QRect(180,70,50,50));
    isDoingCMD = false;
    times = 0;
}

AuthenticationDlg::~AuthenticationDlg()
{
    delete ui;
}

void AuthenticationDlg::on_lineEdit_Password_textEdited(const QString &arg1)
{
    LOGLOG("on_btApply_Login_clicked");
    if(arg1.isEmpty())
    {
        ui->btApply_Login->setEnabled(false);
    }
    else
    {
        ui->btApply_Login->setEnabled(true);
    }
//    ui->label_msg->setText(tr("The default password for this unit is \"888888\".\
//                               For security of your equipment, please change this password."));

//    ui->label_msg->setStyleSheet("QLabel{}");
}

void AuthenticationDlg::on_btApply_Login_clicked()
{
//    LOGLOG("on_btApply_Login_clicked");
    if(ui->lineEdit_Password->text() != NULL)
    {
        qDebug()<<"lineEdit_Password:"<<ui->lineEdit_Password->text()<<"toLatin1:"<<ui->lineEdit_Password->text().toLatin1();
        cmdst_passwd passwd;
        memset(passwd.passwd ,0 ,sizeof(passwd.passwd));
        strcpy(passwd.passwd,ui->lineEdit_Password->text().toLatin1().constData());
        QVariant data;
        data.setValue<cmdst_passwd>(passwd);
        LOGLOG("ui password is :%s" ,passwd.passwd);
        gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PASSWD_confirm,data);

        ui->cycleWidget->show();
        cycle->startAnimation(20);
    }

}

void AuthenticationDlg::cmdResult(int cmd,int result ,QVariant data)
{
    if(UIConfig::LS_CMD_PASSWD_confirm == cmd)
    {
        if(!result)
        {
            *isconfirm = true;
            isDoingCMD = false;
            times = 0;
            close();
        }
        else
        {
            if(!isDoingCMD){
                isDoingCMD = true;
                times = RETRYTIMES;
            }
            if(times > 0){
                times--;
                on_btApply_Login_clicked();
            }
            else {
                *isconfirm = false;
                ui->label_msg->setText(tr("ResStr_Authentication_error__please_enter_the_password_again_"));
                ui->label_msg->setStyleSheet("QLabel{color:red;}");

                isDoingCMD = false;
                times = 0;
            }
        }
        ui->cycleWidget->hide();
        cycle->stopAnimation();
    }
}
