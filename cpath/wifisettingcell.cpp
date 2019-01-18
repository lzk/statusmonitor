#include "wifisettingcell.h"
#include "ui_wifisettingcell.h"
#include "settingwarming.h"
#include "authenticationdlg.h"
#include "uinterface.h"
#define RETRYTIMES 3

WiFiSettingCell::WiFiSettingCell(QWidget *parent, APInfo *info, bool *_islogin, bool isconnected) :
    QStackedWidget(parent),
    ui(new Ui::WiFiSettingCell),
    isConnected(isconnected),
    m_isLogin(false)
{
    ui->setupUi(this);
    this->setCurrentIndex(0);
    if(info != NULL)
    {
        apInfo = *info;
        switch(apInfo.encryType)
        {
        case NO_Securty:
            apInfo.APStatus = tr("ResStr_No_Security"); break;
        case WPA_PSK_TKIP:
            apInfo.APStatus = tr("ResStr_Protected_by_WEP"); break;
        case WPA2_PSK_AES:
            apInfo.APStatus = tr("ResStr_Protected_by_WPA2"); break;
        case Mixed_Mode_PSK:
            apInfo.APStatus = tr("ResStr_Protected_by_Mixed_Mode_PSK"); break;
        default:    break;
        }
    }       //UINT8 encryption ; // 0:No Security 1:WEP 64/128 bit 2.WPA-PSK-TKIP  3. WPA2-PSK-AES 4.Mixed Mode PSK
    else
    {
        apInfo.SSID = tr("SWQA");
        apInfo.APStatus = tr("ResStr_No_Security");
        apInfo.encryType = NO_Securty;
        apInfo.Password = "";
        apInfo.wepKeyID = 0;
    }

    isDisplayPW = false;
    ui->checkBox_visiable->setChecked(isDisplayPW);
    if(!isDisplayPW)
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    else
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);

    isConnected = isconnected;
    initStatus();
    if(NULL != _islogin)
    {
        islogin = _islogin;
    }
    else
    {
        islogin = &m_isLogin;
    }
    isDoingCMD = false;
    times = 0;
}

WiFiSettingCell::~WiFiSettingCell()
{
    delete ui;
}

void WiFiSettingCell::on_btCancel_clicked()
{
    this->setCurrentIndex(0);
    this->resize(QSize(220, 51));
    this->setMinimumHeight(51);
    emit SizeChanged( QSize(220, 154), QSize(211, 51));
}

//
void WiFiSettingCell::on_pushButton_clicked()
{
    this->setCurrentIndex(1);
    this->resize(QSize(220, 154));
    this->setMinimumHeight(154);
    emit SizeChanged( QSize(220, 51), QSize(211, 154));
}

void WiFiSettingCell::on_pushButton_2_clicked()
{
    on_btCancel_clicked();
}

void WiFiSettingCell::on_checkBox_visiable_clicked()
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

void WiFiSettingCell::initStatus()
{
    QRegExp regexp("^[\\x0020-\\x007e]{8,63}$");
    QValidator *validator = new QRegExpValidator(regexp, this);
    ui->lineEdit_Password->setValidator(validator);

    ui->label_SSID1->setText(apInfo.SSID);
    ui->label_SSID2->setText(apInfo.SSID);
    ui->label_APStatus->setText(apInfo.APStatus);
    ui->label_APStatus_2->setText(apInfo.APStatus);
    ui->lineEdit_Password->setText(apInfo.Password);

    if(apInfo.encryType == NO_Securty)
    {
        ui->lineEdit_Password->setDisabled(true);
    }

    if(isConnected)
    {
        ui->label_APStatus->setText(tr("ResStr_Connected"));
        ui->label_APStatus_2->setText(tr("ResStr_Connected"));
        ui->label_StatusIcon->setStyleSheet("QLabel{border-image: url(:/Images/Signal_Connect.png);}");
        ui->label_line1->setStyleSheet("QLabel {border: 1px solid  rgb(105, 185, 18);}");
        ui->label_line2->setStyleSheet("QLabel {border: 1px solid  rgb(105, 185, 18);}");
        apInfo.APStatus.clear();
        apInfo.APStatus = tr("ResStr_Connected");
    }
}

void WiFiSettingCell::on_btConnect_clicked()
{
    int len = ui->lineEdit_Password->text().length();
    if(len<8 && apInfo.encryType != NO_Securty)
    {
        SettingWarming *msgWarm = new SettingWarming(this, tr("ResStr_Msg_3"));
        msgWarm->setWindowTitle("ResStr_Warning");
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint );
        msgWarm->exec();
    }
    else if(apInfo.encryType == NO_Securty)
    {
        emit doingConnect(this);
        //do something here; it is a short time that you can not saw the busyRefresh Label;
        cmdst_wifi_get wifi_para;
        QString ssid;
        QString passwd;
        if(!(*islogin) )
        {
            AuthenticationDlg *dlg = new AuthenticationDlg(this, islogin);
            dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint );
            dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
            dlg->exec();
        }
        if(*islogin)
        {
            ssid = apInfo.SSID;
            passwd = apInfo.Password;
            wifi_para.encryption = apInfo.encryType;
            wifi_para.wepKeyId = apInfo.wepKeyID;
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
           // C_LOG("wifi apply: wifi_para.encryption:%d" ,wifi_para.encryption);

            //wep key id
            if(!(wifi_para.wifiEnable & 1)){
                //_Q_LOG("wifi apply:wifi have not been enable ,enable it");
                wifi_para.wifiEnable &= ~1;
                wifi_para.wifiEnable |= 1;//bit 0
            }

            connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

            QVariant value;
            value.setValue(wifi_para);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_WIFI_apply,value);
        }
        else
        {
 //           disconnect(device ,SIGNAL(signals_cmd_result(int,int)) ,this ,SLOT(slots_cmd_result(int,int)));
            emit connectSuc(this, false);
        }

    }
    else //if(apInfo.encryType == WPA2_PSK_AES || apInfo.encryType == Mixed_Mode_PSK)
    {
        emit doingConnect(this);
        cmdst_wifi_get wifi_para;
        QString ssid;
        QString passwd;
        if(!(*islogin) )
        {
            AuthenticationDlg *dlg = new AuthenticationDlg(this, islogin);
            dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint );
            dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
            dlg->exec();
        }
        if(*islogin)
        {
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
            wifi_para.wepKeyId = 0;

            wifi_para.wifiEnable = 1;

            connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

            QVariant value;
            value.setValue(wifi_para);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_WIFI_apply,value);
        }
        else
        {
            emit connectSuc(this, false);
        }
    }
}

void WiFiSettingCell::changeStatus()
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
        apInfo.APStatus = tr("Protected by ");   break;
    }
    isConnected = false;
    initStatus();
    ui->label_StatusIcon->setStyleSheet("QLabel{border-image: url(:/Images/Signal_Enable.png);}");
    ui->label_line1->setStyleSheet("QLabel { border:1px solid  rgb(181, 181, 181);}");
    ui->label_line2->setStyleSheet("QLabel { border:1px solid  rgb(181, 181, 181);}");
}


APInfo WiFiSettingCell::getAPInfo()
{
    return apInfo;
}

void WiFiSettingCell::tryConnect(APInfo ap)
{
    if(apInfo.SSID == ap.SSID)
    {
        apInfo = ap;
        ui->lineEdit_Password->setText(apInfo.Password);
        on_btConnect_clicked();
    }
}

void WiFiSettingCell::cmdResult(int cmd,int result ,QVariant data)
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
                ui->label_line1->setStyleSheet("QLabel {border: 1px solid  rgb(105, 185, 18);}");
                ui->label_line2->setStyleSheet("QLabel {border: 1px solid  rgb(105, 185, 18);}");
                apInfo.APStatus.clear();
                apInfo.APStatus = tr("ResStr_Connected");
     //           on_btCancel_clicked();
                emit connectSuc(this, true);
            }
            isDoingCMD = false;
            times = 0;
           // _Q_LOG("set wifi success");
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
