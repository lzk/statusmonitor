#include "wifisettingwepcell.h"
#include "ui_wifisettingwepcell.h"
#include "settingwarming.h"
#include "authenticationdlg.h"
#include "jkinterface.h"
#include "uinterface.h"
#define RETRYTIMES 3;

WiFiSettingWEPCell::WiFiSettingWEPCell(QWidget *parent, APInfo *info, bool *_islogin, bool isconnected) :
    QStackedWidget(parent),
    ui(new Ui::WiFiSettingWEPCell),
    isConnected(isconnected),
    m_isLogin(false)
{
    ui->setupUi(this);
    this->setCurrentIndex(0);
    if(info != NULL)
    {
        apInfo = *info;
        apInfo.APStatus = tr("ResStr_Protected_by_WEP");
        isDisplayPW = false;
    }
    else
    {
        apInfo.SSID = tr("SWQA_WEP");
        apInfo.APStatus = tr("ResStr_Protected_by_WEP");
        apInfo.encryType = WEP;
        apInfo.Password = "";
        apInfo.wepKeyID = 0;
        isDisplayPW = false;
    }
    isConnected = isconnected;
    initStatus();

    QRegExp regexp("^[\\x0020-\\x007e]{5,13}$");
    QValidator *validator = new QRegExpValidator(regexp, this);
    ui->lineEdit_Password->setValidator(validator);

    ui->checkBox_visiable->setChecked(isDisplayPW);
    if(!isDisplayPW)
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    else
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);

    if(NULL != _islogin)
    {
        islogin = _islogin;
    }
    else
    {
        islogin = &m_isLogin;
    }
    isDoingCMD = false;
//    times = 0;
}

WiFiSettingWEPCell::~WiFiSettingWEPCell()
{
    delete ui;
}

void WiFiSettingWEPCell::on_btCancel_clicked()
{
    this->setCurrentIndex(0);
    this->resize(QSize(220, 51));
    this->setMinimumHeight(51);
    emit SizeChanged( QSize(220, 210), QSize(220, 51));
}

//
void WiFiSettingWEPCell::on_pushButton_clicked()
{
    this->setCurrentIndex(1);
    this->resize(QSize(220, 210));
    this->setMinimumHeight(210);
    emit SizeChanged( QSize(220, 51), QSize(220, 210));
}

void WiFiSettingWEPCell::on_pushButton_2_clicked()
{
    on_btCancel_clicked();
}

void WiFiSettingWEPCell::on_checkBox_visiable_clicked()
{
    if(ui->checkBox_visiable->isChecked())
    {
        isDisplayPW = true;
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        isDisplayPW = false;
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    }
}

void WiFiSettingWEPCell::initStatus()
{

    ui->label_SSID1->setText(apInfo.SSID);
    ui->label_SSID2->setText(apInfo.SSID);
    ui->label_APStatus->setText(apInfo.APStatus);
    ui->label_APStatus_2->setText(apInfo.APStatus);

    ui->lineEdit_Password->setText(apInfo.Password);

    apInfo.encryType = WEP;
    if(apInfo.encryType == WEP)
    {
        ui->label_keyid->show();
        ui->btKey1->show();
        ui->btKey2->show();
        ui->btKey3->show();
        ui->btKey4->show();
    }
    else
    {
        ui->label_keyid->hide();
        ui->btKey1->hide();
        ui->btKey2->hide();
        ui->btKey3->hide();
        ui->btKey4->hide();
    }
    ui->btKey1->setChecked(false);
    ui->btKey2->setChecked(false);
    ui->btKey3->setChecked(false);
    ui->btKey4->setChecked(false);
    switch(apInfo.wepKeyID)
    {
    case 1: ui->btKey1->setChecked(true);   break;
    case 2: ui->btKey2->setChecked(true);   break;
    case 3: ui->btKey3->setChecked(true);   break;
    case 4: ui->btKey4->setChecked(true);   break;
    default: ui->btKey1->setChecked(true);
             apInfo.wepKeyID = 1;           break;
    }

    if(isConnected)
    {
        ui->label_APStatus->setText(tr("ResStr_Connected"));
        ui->label_APStatus_2->setText(tr("ResStr_Connected"));
        ui->label_StatusIcon->setStyleSheet("QLabel{border-image: url(:/Images/Signal_Connect.png);}");
        ui->label_line1->setStyleSheet("QLabel{border:1px solid  rgb(105, 185, 18);}");
        ui->label_line2->setStyleSheet("QLabel{border:1px solid  rgb(105, 185, 18);}");
        apInfo.APStatus.clear();
        apInfo.APStatus = tr("ResStr_Connected");
    }
}

void WiFiSettingWEPCell::on_btKey1_toggled(bool checked)
{
    if(checked)
        apInfo.wepKeyID = 1;
}


void WiFiSettingWEPCell::on_btKey2_toggled(bool checked)
{
    if(checked)
        apInfo.wepKeyID = 2;
}

void WiFiSettingWEPCell::on_btKey3_toggled(bool checked)
{
    if(checked)
        apInfo.wepKeyID = 3;
}

void WiFiSettingWEPCell::on_btKey4_toggled(bool checked)
{
    if(checked)
        apInfo.wepKeyID = 4;
}

void WiFiSettingWEPCell::on_btConnect_clicked()
{
    int len = ui->lineEdit_Password->text().length();
    if(len < 5)
    {
        SettingWarming *warming = new SettingWarming(this, tr("ResStr_Msg_2"));
        warming->setWindowTitle("ResStr_Warning");
        warming->setWindowFlags(warming->windowFlags() & ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint );
        warming->exec();
        warming->deleteLater();
    }
    else
    {
        emit doingConnect(this);
        if(!(*islogin ))
        {
            AuthenticationDlg *dlg = new AuthenticationDlg(this, islogin);
            dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint );
            dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
            dlg->exec();
            dlg->deleteLater();
        }
        if(*islogin)
        {
            cmdst_wifi_get wifi_para;
            QString ssid;
            QString passwd;

            ssid = apInfo.SSID;
            passwd = ui->lineEdit_Password->text();
            //ssid
            if(strlen(ssid.toUtf8()) >= 32)
                memcpy(wifi_para.ssid ,ssid.toUtf8() ,32);
            else{
                memset(wifi_para.ssid ,0 ,32);
                strcpy(wifi_para.ssid ,ssid.toUtf8());
            }

            //passwd
            if(strlen(passwd.toLatin1()) >= 64)
                memcpy(wifi_para.pwd ,passwd.toLatin1() ,64);
            else{
                memset(wifi_para.pwd ,0 ,64);
                strcpy(wifi_para.pwd ,passwd.toLatin1());
            }

            //encryption type
            wifi_para.encryption = apInfo.encryType;

            //C_LOG("wifi apply: wifi_para.encryption:%d" ,wifi_para.encryption);
            wifi_para.wepKeyId = apInfo.wepKeyID;
//            wifi_para.channel = 0;

            wifi_para.wifiEnable = 1;
            connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

            QVariant value;
            value.setValue<cmdst_wifi_get>(wifi_para);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_WIFI_apply,value);
        }
        else
        {
            emit connectSuc(this, false);
        }
    }
}

//turn the status to disconnect
void WiFiSettingWEPCell::changeStatus()
{
    switch(apInfo.encryType)
    {
    case NO_Securty:
        apInfo.APStatus = tr("ResStr_No_Security"); break;
    case WEP:
        apInfo.APStatus = tr("ResStr_Protected_by_WEP"); break;
    case WPA2_PSK_AES:
        apInfo.APStatus = tr("ResStr_Protected_by_WPA2"); break;
    case Mixed_Mode_PSK:
        apInfo.APStatus = tr("ResStr_Protected_by_Mixed_Mode_PSK"); break;
//    case WPA_PSK_TKIP:
//        apInfo.APStatus = tr("Protected by WPA_PSK_TKIP"); break;
    default:
        apInfo.APStatus = tr("Protected by "); break;
    }
    isConnected = false;
    initStatus();
    ui->label_StatusIcon->setStyleSheet("QLabel{border-image: url(:/Images/Signal_Enable.png);}");
    ui->label_line1->setStyleSheet("QLabel{border:1px solid  rgb(181, 181, 181);}");
    ui->label_line2->setStyleSheet("QLabel{border:1px solid  rgb(181, 181, 181);}");
}

APInfo WiFiSettingWEPCell::getAPInfo()
{
    return apInfo;
}

void WiFiSettingWEPCell::tryConnect(APInfo ap)
{
    if(apInfo.SSID == ap.SSID)
    {
        apInfo = ap;
        ui->lineEdit_Password->setText(apInfo.Password);
        on_btConnect_clicked();
    }
}

void WiFiSettingWEPCell::cmdResult(int cmd,int result ,QVariant data)
{
    if(UIConfig::LS_CMD_WIFI_apply == cmd)
    {
        if(!result)
        {
            if(1)
            {
                ui->label_APStatus->setText(tr("ResStr_Connected"));
                ui->label_APStatus_2->setText(tr("ResStr_Connected"));
                ui->label_StatusIcon->setStyleSheet("QLabel{border-image: url(:/Images/Signal_Connect.png);}");
                ui->label_line1->setStyleSheet("QLabel{border:1px solid  rgb(105, 185, 18);}");
                ui->label_line2->setStyleSheet("QLabel{border:1px solid  rgb(105, 185, 18);}");
                apInfo.APStatus.clear();
                apInfo.APStatus = tr("ResStr_Connected");
     //           on_btCancel_clicked();
                emit connectSuc(this, true);
            }
            isDoingCMD = false;
            times = 0;
            //_Q_LOG("set wifi success");
        }
        else
        {
            if(!isDoingCMD){
                isDoingCMD = false;
                times = RETRYTIMES;
            }
            if(times > 0){
                times--;
                on_btConnect_clicked();
            }
            else{
                emit connectSuc(this, false);
                isDoingCMD = false;
            }
        }
        if(!isDoingCMD)
            disconnect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));
    }
}
