#include "membercenterwidget.h"
#include "ui_membercenterwidget.h"
#include "membercenter/userlogin.h"
#include "membercenter/about.h"
#include "membercenter/experiencepro.h"
#include "jsonparser/parser.h"
#include "qmessagebox.h"
#include "qdesktopservices.h"
#include <qstringlist.h>
#include "uinterface.h"

MemberCenterWidget::MemberCenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberCenterWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    m_strKey = "86c02972fba047b0b0a9adb8123029fb";

    ui->btChInfo->setDisabled(true);
    ui->changeMsg->setStyleSheet("QLabel{background-color: rgb(235, 235, 235);color:gray}");

    crmTimer = new QTimer(this);
    connect(crmTimer,SIGNAL(timeout()),this,SLOT(uploadCRM()));

    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)), this ,SLOT(cmdResult(int,int,QVariant)));
}

MemberCenterWidget::~MemberCenterWidget()
{
    delete ui;
}

void MemberCenterWidget::on_btLogin_clicked()
{
    UserLogin *login = new UserLogin(this);
    login->exec();

    if(login->isLogin())
    {
        loginPhone = login->getPhone();
        currentUser.mobile = loginPhone;
        ui->login_name->setText( loginPhone );
        ui->btLogin->setDisabled(true);
        ui->login_arrow->hide();
        isLogin = true;
        //userInfo(true);
        ui->btloginImg->setStyleSheet("QPushButton{"
                                       "border-image: url(:/Images/Logon_Active.png);}"
                                       "QPushButton:pressed{"
                                       "border-image: url(:/Images/Logon_Normal.png);}");
        ui->btloginImg2->setStyleSheet("QPushButton{"
                                       "border-image: url(:/Images/Logon_Active.png);}"
                                       "QPushButton:pressed{"
                                       "border-image: url(:/Images/Logon_Normal.png)}");
        bt->setStyleSheet("QPushButton{"
                          "border-image: url(:/Images/Logon_Active.png);}"
                          "QPushButton:pressed{"
                          "border-image: url(:/Images/Logon_Normal.png)}");
        ui->btChInfo->setEnabled(true);
        ui->changeMsg->setStyleSheet("QLabel{background-color: rgb(235, 235, 235);}");
    }

    login->deleteLater();
}

void MemberCenterWidget::on_btAbout_clicked()
{
    About *about = new About(this);
    about->exec();
    about->deleteLater();
}

void MemberCenterWidget::on_btCancel_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MemberCenterWidget::getUserInfo()
{
    QString baseUrl = "http://crm.iprintworks.cn/api/app_getuserinfo";
    QUrl url(baseUrl);

    QDateTime dateTime;
    QString time = dateTime.currentDateTime().toString("yyyyMMddHHmmss");

    QString text = loginPhone;

    QString str = QString("%0%1%2").arg(text).arg(time).arg(m_strKey);
//    qDebug()<<str;

    QString md5;
    QByteArray bb;
#if QT_VERSION_MAJOR > 4
    bb = QCryptographicHash::hash(str.toLocal8Bit(),QCryptographicHash::Md5);
#else
    bb = QCryptographicHash::hash(str.toAscii(),QCryptographicHash::Md5);
#endif
    md5.append(bb.toHex());

    QByteArray post_data;
    QString post_str = QString("mobile=%0&time=%1&Sign=%2").arg(text).arg(time).arg(md5);
//    qDebug()<<post_str;
#if QT_VERSION_MAJOR > 4
    post_data = post_str.toLocal8Bit();
#else
    post_data = post_str.toAscii();
#endif

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinish_get(QNetworkReply*)));

    QNetworkRequest *req = new QNetworkRequest();

    req->setUrl(url);

    req->setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded; charset=UTF-8");
    req->setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

    manager->post(*req,post_data);

}

void MemberCenterWidget::replyFinish_get(QNetworkReply* reply)
{
    QString strJsonText = reply->readAll();
    qDebug()<<"replyFinish_get"<<strJsonText;

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(strJsonText.toUtf8(),&ok).toMap();

    if (result["success"].toBool())
    {
        QVariantMap user = result["user"].toMap();

        if(user["realName"].toString() != NULL)
        {
            ui->le_name->setText(user["realName"].toString());
        }
        else
        {
            ui->le_name->setText("");
        }

        QDate birthDate = user["birthDate"].toDate();
        ui->spinBox->setValue(birthDate.year());
        ui->spinBox_M->setValue(birthDate.month());
        ui->spinBox_D->setValue(birthDate.day());

        if(user["sex"].toString() != NULL)
        {
            if(user["sex"].toString() == "男")
            {
                ui->btFemale->setChecked(false);
                ui->btMan->setChecked(true);
            }
            else
            {
                ui->btFemale->setChecked(true);
                ui->btMan->setChecked(false);
            }
        }
        else
        {
            ui->btFemale->setChecked(false);
            ui->btMan->setChecked(false);
        }

        if(user["email"].toString() != NULL)
        {
            ui->le_mail->setText(user["email"].toString());
        }
        else
        {
            ui->le_mail->setText("");
        }

        if(user["address"].toString() != NULL)
        {
            ui->le_addr->setText(user["address"].toString());
        }
        else
        {
            ui->le_addr->setText("");
        }
    }

    ui->btChInfo->setEnabled(true);
    ui->stackedWidget->setCurrentIndex(0);

}

QString MemberCenterWidget::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();

    int nCnt = nets.count();
    QString strMacAddr = "";

    for(int i = 0; i < nCnt; i++)
    {
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

void MemberCenterWidget::setUserInfo()
{
    QString baseUrl = "http://crm.iprintworks.cn/api/app_userinfo";
    QUrl url(baseUrl);

    QString realName = ui->le_name->text();

    QDate *birthDate = new QDate;
    birthDate->setDate(ui->spinBox->value(),ui->spinBox_M->value(),ui->spinBox_D->value());
    QString strBirthDay = birthDate->toString("yyyy-MM-dd");
    qDebug()<<"strBirthDay:"<<strBirthDay;

    int nSex = 0x01;

    if(ui->btFemale->isChecked())
    {
        nSex = 0x00;
    }

    QString strEmail = ui->le_mail->text();
    QString strAddress = ui->le_addr->text();

    QDateTime dateTime;
    QString time = dateTime.currentDateTime().toString("yyyyMMddHHmmss");

    QString strMacAddr = MemberCenterWidget::getHostMacAddress();
    qDebug()<<"strMacAddr:"<<strMacAddr;

    QString md5_str = QString("%0%1%2").arg(strMacAddr).arg(time).arg(m_strKey);
    qDebug()<<"md5_str:"<<md5_str;

    QString strSign;
    QByteArray bb;
#if QT_VERSION_MAJOR > 4
    bb = QCryptographicHash::hash(md5_str.toLocal8Bit(),QCryptographicHash::Md5);
#else
    bb = QCryptographicHash::hash(md5_str.toAscii(),QCryptographicHash::Md5);
#endif
    strSign.append(bb.toHex());

    QString post_str = QString("MobileCode=%0&Mobile=%1&DeviceBrand%2&DeviceModel=%3&Truename=%4&Birthdate=%5&Sex=%6&Email=%7&Address=%8&time=%9&sign=%10")
            .arg(strMacAddr)
            .arg(loginPhone)
            .arg("")
            .arg("")
            .arg(realName)
            .arg(strBirthDay)
            .arg(nSex)
            .arg(strEmail)
            .arg(strAddress)
            .arg(time)
            .arg(strSign);
    qDebug()<<"post_str:"<<post_str;
#if QT_VERSION_MAJOR > 4
    QByteArray post_data = post_str.toLocal8Bit();
#else
    QByteArray post_data = post_str.toAscii();
#endif

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinish_set(QNetworkReply*)));

    QNetworkRequest *req = new QNetworkRequest();

    req->setUrl(url);

    req->setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded; charset=UTF-8");
    req->setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

    manager->post(*req,post_data);
}

void MemberCenterWidget::replyFinish_set(QNetworkReply* reply)
{
    QString strJsonText = reply->readAll();
    qDebug()<<"replyFinish_set"<<strJsonText.toUtf8();

    QJson::Parser parser;
    bool ok;
    QVariantMap result = parser.parse(strJsonText.toUtf8(),&ok).toMap();
    if(result["success"].toBool() == true)
    {
        ui->le_name->setText(result["message"].toString());
        ui->btLogin->setText(QString("%0(%1)").arg(ui->le_name->text()).arg(loginPhone));
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        QMessageBox *message = new QMessageBox(this);
        message->setWindowTitle(QString::fromLocal8Bit("错误"));
        message->setIconPixmap(QPixmap(":/Images/Warning.tif"));
        message->setText(QString::fromLocal8Bit("修改账户信息失败。"));
        message->exec();
    }
}

void MemberCenterWidget::on_btChInfo_clicked()
{
    getUserInfo();
    ui->btChInfo->setEnabled(false);
}

void MemberCenterWidget::on_btExpe_clicked()
{
    ExperiencePro *exp = new ExperiencePro(this);
    exp->exec();

    if(exp->isStartCRM())
    {
        qDebug()<<"start crm";
        MemberCenterWidget::uploadCRM();
        crmTimer->start(30*60*1000);//30min
    }
    else
    {
        qDebug()<<"stop crm";
        crmTimer->stop();
    }

    exp->deleteLater();
}

void MemberCenterWidget::uploadCRM()
{
    qDebug()<<"uploadcrm";
    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_Get_UserCenterInfo);
}

void MemberCenterWidget::cmdResult(int cmd,int result,QVariant data)
{
    switch(cmd)
    {
    case UIConfig::LS_CMD_PRN_Get_UserCenterInfo:
        if(!result)
        {
            struct_user_center userCenter = data.value<struct_user_center>();
            QString inkLevel = "0";
            QString printerDataStr = QString("[{{\"printermodel\":\"%0\",\"printerid\":\"%1\",\"tonerid\":\"%2\",\"totalprint\":\"%3\",\"inklevel\":\"%4\"}}]")
                    .arg(userCenter.strPrinterModel)
                    .arg(userCenter.user_center._2ndSerialNO)
                    .arg(userCenter.user_center._SerialNO4AIO)
                    .arg(userCenter.user_center._totalCounter)
                    .arg(inkLevel);

            QString strMacAddr = MemberCenterWidget::getHostMacAddress();
            QDateTime dateTime;
            QString time = dateTime.currentDateTime().toString("yyyyMMddHHmmss");
            QString md5_str = QString("%0%1%2").arg(strMacAddr).arg(time).arg(m_strKey);

            QString strPlatform = "MacPC";
            QString strVersion = "1.0.0.1";


            QString post_str = QString("{{\"devicecode\":\"%0\",\"mobile\":\"%1\",\"platform\":\"%2\",\"version\":\"%3\",\"time\":\"%4\",\"sign\":\"%5\",\"location\":\"\",\"printerdata\":%6}}")
                    .arg(strMacAddr)
                    .arg(loginPhone)
                    .arg(strPlatform)
                    .arg(strVersion)
                    .arg(time)
                    .arg(md5_str)
                    .arg(printerDataStr);
            qDebug()<<post_str;

#if QT_VERSION_MAJOR > 4
    QByteArray post_data = post_str.toLocal8Bit();
#else
    QByteArray post_data = post_str.toAscii();
#endif

             QString baseUrl = "http://crm.iprintworks.cn/api/printerinfo";//debug
             QUrl url(baseUrl);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinish_uploadCRM(QNetworkReply*)));

             QNetworkRequest *req = new QNetworkRequest();

             req->setUrl(url);

             req->setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded; charset=UTF-8");
             req->setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

             manager->post(*req,post_data);

        }
        break;

    default:
        break;

    }
}

void MemberCenterWidget::replyFinish_uploadCRM(QNetworkReply *reply)
{
    QString strJsonText = reply->readAll();
    qDebug()<<"replyFinish_uploadCRM"<<strJsonText;
}

void MemberCenterWidget::setSW(QStackedWidget* _sw, QPushButton * _bt)
{
    sw = _sw;
    bt = _bt;
}

void MemberCenterWidget::on_pushButton_clicked()
{
    if(sw){
        sw->setCurrentIndex(0);
    }
}

void MemberCenterWidget::on_btApply_clicked()
{
    setUserInfo();
}

void MemberCenterWidget::on_btProduct_clicked()
{
    QDesktopServices::openUrl(QUrl("http://ibase.lenovoimage.com/home_abc.aspx"));
}
