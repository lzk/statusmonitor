#include "userlogin.h"
#include "ui_userlogin.h"
#include "qdatetime.h"
#include "qurl.h"
#include "jsonparser/parser.h"

UserLogin::UserLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogin)
{
    ui->setupUi(this);
    m_strKey = "86c02972fba047b0b0a9adb8123029fb";
    m_loginSuccess = false;
    acTimer = new QTimer(this);
    connect(acTimer,SIGNAL(timeout()),this,SLOT(showTimelimit()));
}

UserLogin::~UserLogin()
{
    delete ui;
}

void UserLogin::on_bt_login_clicked()
{
    qDebug()<<"on_bt_login_clicked";
    if(ui->le_userName->text() == "")
    {
        return;
    }

    QString baseUrl = "http://function.iprintworks.cn:8001/smsauth/authCode.php";
    QUrl url(baseUrl);

    QString strPhoneNumber = ui->le_userName->text();
    QString strVerifyCode = ui->le_autoCode->text();

    QString post_str = QString("phoneNum=%0&authCode=%1").arg(strPhoneNumber).arg(strVerifyCode);
#if QT_VERSION > 0x050000
    QByteArray post_data = post_str.toLocal8Bit();
#else
    QByteArray post_data = post_str.toAscii();
#endif

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinish_check(QNetworkReply*)));

    QNetworkRequest req;

    req.setUrl(url);

    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded; charset=UTF-8");
    req.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

    manager->post(req,post_data);
}

void UserLogin::replyFinish_check(QNetworkReply* reply)
{
    QString strJsonText = reply->readAll();
    qDebug()<<"replyFinish_check"<<strJsonText;

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(strJsonText.toUtf8(),&ok).toMap();

    if(ok)
    {
        if(result["success"].toBool())
        {
            m_loginSuccess = true;
            close();
        }
        else
        {
            ui->labMsg->setText(tr("ResStr_Msg_8"));
        }
    }
    reply->deleteLater();
}

void UserLogin::on_bt_getAuthCode_clicked()
{
    qDebug()<<"on_bt_getAuthCode_clicked";
    QString baseUrl = "http://function.iprintworks.cn:8001/smsauth/mt_u.php";
    QUrl url(baseUrl);

    QString strPhoneNumber = ui->le_userName->text();

    QString str = QString("phoneNum=%0").arg(strPhoneNumber);
    qDebug()<<str;
#if QT_VERSION > 0x050000
    QByteArray post_data = str.toLocal8Bit();
#else
    QByteArray post_data = str.toAscii();
#endif
    qDebug()<<post_data;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinish_send(QNetworkReply*)));

    QNetworkRequest req;

    req.setUrl(url);

    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded; charset=UTF-8");
    req.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

    manager->post(req,post_data);
}

void UserLogin::showTimelimit()
{
    qDebug()<<acTime;
    if(acTime != 0)
    {
        acTime--;
        QString num = QString::number(acTime);
        ui->bt_getAuthCode->setText(num);
    }
    else
    {
        ui->bt_getAuthCode->setEnabled(true);
        ui->bt_getAuthCode->setText(tr("ResStr_Get_Verification_Code"));
        acTimer->stop();
    }
}

void UserLogin::replyFinish_send(QNetworkReply* reply)
{
    QString strJsonText = reply->readAll();
    qDebug()<<"replyFinish_send"<<strJsonText;

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(strJsonText.toUtf8(),&ok).toMap();

    if(ok)
    {
        if(result["success"].toBool())
        {
            ui->labMsg->setText(tr("ResStr_Msg_5"));
            acTime = 60;
            QString num = QString::number(acTime);
            ui->bt_getAuthCode->setText(num);
            ui->bt_getAuthCode->setEnabled(false);
            acTimer->start(1000);
            return;
        }
    }

    ui->labMsg->setText(tr("ResStr_Msg_6"));
    reply->deleteLater();
}

QString UserLogin::getPhone()
{
    QString phone = ui->le_userName->text();
    return phone;
}

bool UserLogin::isLogin()
{
    return m_loginSuccess;
}
