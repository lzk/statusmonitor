#include "settingsstackedwidget.h"
#include "ui_settingsstackedwidget.h"
#include "uinterface.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "authenticationdlg.h"
#include "settingwarming.h"
#include "qdebug.h"

#define RETRYTIMERS 3;

SettingsStackedWidget::SettingsStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SettingsStackedWidget)
{
    ui->setupUi(this);
    this->setCurrentIndex(0);

    scrollArea = new QScrollArea(ui->WiFi);
    scrollArea->setGeometry(QRect(13, 14, 235, 274));
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setStyleSheet("QScrollBar:vertical"
                            "{"
                            "width:10px;"
                            "background:rgba(0,0,0,0%);"
                            "margin:0px,0px,0px,0px;"
                            "padding-top:12px;"
                            "padding-bottom:12px;"
                            "}"
                            "QScrollBar::handle:vertical"
                            "{"
                            "width:8px;"
                            "background:rgba(0,0,0,25%);"
                            " border-radius:5px;"
                            "min-height:20;"
                            "}"
                            "QScrollBar::handle:vertical:hover"
                            "{"
                            "width:8px;"
                            "background:rgba(0,0,0,50%);"
                            " border-radius:5px;"
                            "min-height:20;"
                            "}"
                            "QScrollBar::add-line:vertical"
                            "{"
                            "height:12px;width:12px;"
                            "subcontrol-position:bottom;"
                            "}"
                            "QScrollBar::sub-line:vertical"
                            "{"
                            "height:12px;width:12px;"
                            "subcontrol-position:top;"
                            "}"
                            "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                            "{"
                            "background:rgba(0,0,0,0%);"
                            "border-radius:4px;"
                            "}"
                            );
    isLogn = false;     //isLogn :default false; false:did not comfirm the pw in the device;
    titelCell = new WlanTitleCell(scrollArea, true, &isLogn);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(titelCell);
    timer1 = new QTimer;
    initAP();
    initIP();
    initPowerSave();
    initPassword();
    initAdvanceSetting(userconfig, true);
    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)), this ,SLOT(cmdResult(int,int,QVariant)));

    isDoingCMD = false;
    retryTimes = 0;
}

SettingsStackedWidget::~SettingsStackedWidget()
{
    delete ui;
}

void SettingsStackedWidget::cmdResult(int cmd,int result,QVariant data)
{
    if(this->isEnabled()){
        switch(cmd)
        {
        case UIConfig::CMD_WIFI_refresh_plus:
            LOGLOG("CMD_WIFI_refresh_plus enter");
            if(!result || WarmingUp == result){ //when the machine is warmming up, the err will be STATUS_WarmingUp
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    gUInterface->setCurrentPrinterCmd(UIConfig::CMD_WIFI_refresh_plus);
                }
                else{
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
                emit cycleStop();
            break;
        case UIConfig::LS_CMD_WIFI_Get_SoftAp:
            if(!result || WarmingUp == result)
            {
                qDebug()<<QString(softap.ssid).left(32)<<":"<<softap.pwd;
                softap = data.value<cmdst_softap>();
                ui->lineEdit_SSID_AP->setText(QString(softap.ssid).left(32));
                ui->lineEdit_Password_AP->setText(QString(softap.pwd));
                isAPOpen = 6!=(softap.wifiEnable & 6);          //bit0: Wi-Fi Enable, bit1: GO Enable, bit2: P2P Enable
                on_btAPOpen_clicked();
                isDoingCMD = false;
                retryTimes = 0;
            }
            else
            {
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_WIFI_Get_SoftAp);
                }
                else{
                    ui->lineEdit_SSID_AP->clear();                  //clear the data last time;
                    ui->lineEdit_Password_AP->clear();
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
                emit cycleStop();
            break;
        case UIConfig::LS_CMD_WIFI_Set_SoftAp:
            if(!result){
                LOGLOG("set sofAp successful");
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                LOGLOG(" fail to set sofAp");
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    on_btApply_AP_clicked();
                }
                else{
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
            {
                emit cycleStop();
                QString deviceMsg;
                if(result)
                    deviceMsg = tr("ResStr_Msg_1");
                else
                    deviceMsg = tr("ResStr_Setting_Fail");
                gUInterface->setDeviceMsgFrmUI(deviceMsg,result);
            }
            break;
        case UIConfig::LS_CMD_NET_GetV4:
            if(!result || WarmingUp == result)
            {
                info_ipv4 = data.value<net_info_st>();
                info_ipv4_orn = info_ipv4;
                initIP();
                isDoingCMD = false;
                retryTimes = 0;
            }
            else
            {
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_GetV4);
                }
                else{
                    isDoingCMD = false;
                    ui->lineEdit_IPAddressv4->setText("0.0.0.0");
                    ui->lineEdit_Gatewayv4->setText("0.0.0.0");
                    ui->lineEdit_Submaskv4->setText("0.0.0.0");
                }
            }
            if(!isDoingCMD) {
                emit cycleStop();
            }
            break;

        case UIConfig::LS_CMD_NET_SetV4:
            if(!result )
            {
                info_ipv4 = info_ipv4_orn;
                LOGLOG("set ipv4 successful");
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    on_btApply_IPConfig_clicked();
                }
                else{
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
            {
                emit cycleStop();
                QString deviceMsg;
                if(result)
                    deviceMsg = tr("ResStr_Msg_1");
                else
                    deviceMsg = tr("ResStr_Setting_Fail");
                gUInterface->setDeviceMsgFrmUI(deviceMsg,result);
            }
            break;

        case UIConfig::LS_CMD_PRN_PSaveTime_Get:
            if(!result || WarmingUp == result) {
                psavetm = data.value<cmdst_PSave_time>();
                QString str;
                str.setNum(psavetm);
                ui->lineEdit_timeout->setText(str);
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_PSaveTime_Get);
                }
                else{
                    isDoingCMD = false;
                }
            }
            emit cycleStop();
            break;
        case UIConfig::LS_CMD_PRN_PSaveTime_Set:
            if(!result) {
                LOGLOG("set power save time successful");
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {                                      //set the retry times
                if(!isDoingCMD)
                {
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    on_btApply_Timeout_clicked();
                }
                else{
                    isDoingCMD = false;
                    LOGLOG("fail to set power save time");
                }
            }
            if(!isDoingCMD)
            {
                emit cycleStop();
                QString deviceMsg;
                if(result)
                    deviceMsg = tr("ResStr_Setting_Successfully_");
                else
                    deviceMsg = tr("ResStr_Setting_Fail");
                gUInterface->setDeviceMsgFrmUI(deviceMsg,result);
            }

            break;

        case UIConfig::LS_CMD_PRN_Get_UserConfig:
            if(!result || WarmingUp == result) {
                userconfig = data.value<cmdst_userconfig>();
                userconfig_orn = userconfig;
                initAdvanceSetting(userconfig, false);
                isDoingCMD = false;
                retryTimes = 0;
            }
            else{
                if(!isDoingCMD){
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_Get_UserConfig);
                }
                else{
                    isDoingCMD = false;
                    initAdvanceSetting(userconfig, true);
                }
            }
            if(!isDoingCMD)
                emit cycleStop();
            break;
        case UIConfig::LS_CMD_PRN_Set_UserConfig:
            if(!result)
            {
                userconfig = userconfig_orn;
                //initAdvanceSetting(userconfig, false);
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                if(!isDoingCMD){
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    on_btApply_userconfig_clicked();
                }
                else{
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
            {
                emit cycleStop();
                QString deviceMsg;
                if(result)
                    deviceMsg = tr("ResStr_Setting_Successfully_");
                else
                    deviceMsg = tr("ResStr_Setting_Fail");
                gUInterface->setDeviceMsgFrmUI(deviceMsg,result);
            }
            break;
        case UIConfig::LS_CMD_PRN_FusingScReset:
            if(!result)
            {
                SettingWarming *warming = new SettingWarming(0, tr("ResStr_Please_turn_off_the_printer_until_it_cools_to_room_temperature"), true);
                warming->setWindowTitle("ResStr_Prompt");

                warming->setWindowFlags(warming->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                    & ~Qt::WindowMinimizeButtonHint);
                warming->exec();
                warming->deleteLater();
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                if(!isDoingCMD){
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    on_btErrorClear_clicked();
                }
                else{
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
            {
                emit cycleStop();
                QString deviceMsg;
                if(result)
                    deviceMsg = tr("ResStr_Setting_Successfully_");
                else
                    deviceMsg = tr("ResStr_Setting_Fail");
                gUInterface->setDeviceMsgFrmUI(deviceMsg,result);
            }
            break;
        case UIConfig::LS_CMD_PASSWD_set:
            if(!result)
            {
                isDoingCMD = false;
                retryTimes = 0;
            }
            else {
                if(!isDoingCMD){
                    isDoingCMD = true;
                    retryTimes = RETRYTIMERS;
                }
                if(retryTimes > 0){
                    retryTimes--;
                    on_btApply_clicked();
                }
                else{
                    isDoingCMD = false;
                }
            }
            if(!isDoingCMD)
                emit cycleStop();
            break;
        default: break;
        }
    }else{
        emit cycleStop();
        isDoingCMD = false;
        retryTimes = 0;
    }
}

void SettingsStackedWidget::changeStackIndex(int index)
{
    switch(index)
    {
    case 0:                                     //wifi setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            titelCell->setCurrentIndex(0);
            titelCell->on_btFlesh_clicked();
//            gUInterface->setCurrentPrinterCmd(UIConfig::CMD_WIFI_refresh_plus);
            emit cycleStart();
        }
        break;
    case 1:                                     //softAP setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_WIFI_Get_SoftAp);
            emit cycleStart();
        }
        break;
    case 2:                                    // ip settting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            QVariant data;
            data.setValue<net_info_st>(info_ipv4);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_GetV4);
            emit cycleStart();
        }
        break;
    case 3:                                    // ip settting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
        }
        break;
    case 4:                                    // power save time setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_PSaveTime_Get);
            emit cycleStart();
        }
        break;
    case 5:                                   // advance setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_Get_UserConfig);
            emit cycleStart();
        }
        break;
    case 6:                                   //new password setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
        }
        break;
    default:
        break;
    }
}

void SettingsStackedWidget::initAP()
{
   // if(NULL != device)
    {
       // device->
    }
    isAPOpen = true;
    ui->btAPOpen->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
    ui->lineEdit_SSID_AP->setText(tr(""));
    ui->lineEdit_SSID_AP->setCursorPosition(1);
    ui->lineEdit_Password_AP->clear();

    QRegExp rx1("^[0-9a-zA-Z]{1,32}$");
    QRegExp rx2("^[\\x0020-\\x007e]{8,64}$");
    QRegExpValidator *validator1 = new QRegExpValidator(rx1, this);
    QRegExpValidator *validator2 = new QRegExpValidator(rx2, this);
    ui->lineEdit_SSID_AP->setValidator(validator1);
    ui->lineEdit_Password_AP->setValidator(validator2);
}

void SettingsStackedWidget::initIP()
{
    ui->label_ipaddr_error->hide();
    ui->label_gateway_error->hide();
    ui->label_sumask_error->hide();
    ui->label_server_error->hide();
    ui->lineEdit_IPAddressv4->installEventFilter(this);
    ui->lineEdit_Gatewayv4->installEventFilter(this);
    ui->lineEdit_Submaskv4->installEventFilter(this);
    ui->lineEdit_server->installEventFilter(this);

    if(0 == info_ipv4.SubnetMask[0])            //if true, that means cmd_get_ipv4 did not success;
    {
        info_ipv4.IPAddressMode = 3;            //to set default value;
        info_ipv4.IPMode = 1;
    }
//    if(0==info_ipv4.IPMode)                   /*****IPV4 mode *******/
//    {
        if(info_ipv4.IPAddressMode < 4)         //DHCP mode
        {
             ui->btDHCP->setChecked(true);
             on_btDHCP_toggled(true);
             QString text = "%1.%2.%3.%4";
             text = text.arg(info_ipv4.IPAddress[0]).arg(info_ipv4.IPAddress[1]).\
                                        arg(info_ipv4.IPAddress[2]).arg(info_ipv4.IPAddress[3]);
             ui->lineEdit_IPAddressv4->setText(tr(text.toLatin1()));
             text = "%1.%2.%3.%4";
             text = text.arg(info_ipv4.GatewayAddress[0]).arg(info_ipv4.GatewayAddress[1]).\
                                        arg(info_ipv4.GatewayAddress[2]).arg(info_ipv4.GatewayAddress[3]);
             ui->lineEdit_Gatewayv4->setText(tr(text.toLatin1()));
             text = "%1.%2.%3.%4";
             text = text.arg(info_ipv4.SubnetMask[0]).arg(info_ipv4.SubnetMask[1]).\
                                        arg(info_ipv4.SubnetMask[2]).arg(info_ipv4.SubnetMask[3]);
             ui->lineEdit_Submaskv4->setText(tr(text.toLatin1()));
        }
        else if(4 == info_ipv4.IPAddressMode)       //static ip mode
        {
            ui->btStatic->setChecked(true);
            on_btStatic_toggled(true);
            QString text = "%1.%2.%3.%4";
            text = text.arg(info_ipv4.IPAddress[0]).arg(info_ipv4.IPAddress[1]).\
                                       arg(info_ipv4.IPAddress[2]).arg(info_ipv4.IPAddress[3]);
            ui->lineEdit_IPAddressv4->setText(tr(text.toLatin1()));
            text = "%1.%2.%3.%4";
            text = text.arg(info_ipv4.GatewayAddress[0]).arg(info_ipv4.GatewayAddress[1]).\
                                       arg(info_ipv4.GatewayAddress[2]).arg(info_ipv4.GatewayAddress[3]);
            ui->lineEdit_Gatewayv4->setText(tr(text.toLatin1()));
            text = "%1.%2.%3.%4";
            text = text.arg(info_ipv4.SubnetMask[0]).arg(info_ipv4.SubnetMask[1]).\
                                       arg(info_ipv4.SubnetMask[2]).arg(info_ipv4.SubnetMask[3]);
            ui->lineEdit_Submaskv4->setText(tr(text.toLatin1()));
        }
//    }
}

void SettingsStackedWidget::initPowerSave()
{
    ui->lineEdit_timeout->setText("1");
    QRegExp rx4("^[0-9]{0,3}$");
    QRegExpValidator *validator4 = new QRegExpValidator(rx4, this);
    ui->lineEdit_timeout->setValidator(validator4);
    ui->lineEdit_timeout->installEventFilter(this);

    ui->label_timeout_error->hide();

    connect(timer1, SIGNAL(timeout()), this, SLOT(on_timer1_timeout())); //this timer work for add  & reduce button;
}

void SettingsStackedWidget::initPassword()
{
    QRegExp rx4("^[\\x0020-\\x0076]{0,32}$");
    QRegExpValidator *validator4 = new QRegExpValidator(rx4, this);
    ui->lineEdit_newPassWord->setValidator(validator4);
    ui->lineEdit_confirm->setValidator(validator4);
}


void SettingsStackedWidget::on_btTimeReduce_clicked()
{
    int val = ui->lineEdit_timeout->text().toInt();
    if(val>1)
    {
        val--;
        ui->lineEdit_timeout->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_btTimeAdd_clicked()
{
    int val = ui->lineEdit_timeout->text().toInt();
    if(val<30)
    {
        val++;
        ui->lineEdit_timeout->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_lineEdit_timeout_textEdited(const QString &arg1)
{
    QRegExp rx2("^[0]{0,1}[0-2]{0,1}[1-9]{1}|10|20|30$");
    if( !rx2.exactMatch(arg1) )
    {
        ui->label_timeout_error->show();
        ui->lineEdit_timeout->setStyleSheet("QLineEdit{border:2px solid red;border-radius:15px;}");
        ui->btTimeAdd->setDisabled(true);
        ui->btTimeReduce->setDisabled(true);
        ui->btApply_Timeout->setDisabled(true);

    }
    else
    {
        ui->label_timeout_error->hide();
        ui->lineEdit_timeout->setStyleSheet("QLineEdit{border:transparent;}");
        ui->btTimeAdd->setEnabled(true);
        ui->btTimeReduce->setEnabled(true);
        ui->btApply_Timeout->setEnabled(true);
    }
}

void SettingsStackedWidget::on_timer1_timeout()
{
    if(timercounter > 5)
    {
        if(isADD)
        {
            on_btTimeAdd_clicked();
        }
        else
        {
            on_btTimeReduce_clicked();
        }
    }
    timercounter++;
}

void SettingsStackedWidget::initAdvanceSetting(cmdst_userconfig config, bool isFirst)
{
    if(isFirst)             //if ture, set the default value;
    {
        QRegExp rx1("-{0,1}[0-9]{1,2}");
        QRegExpValidator *validator1 = new QRegExpValidator(rx1,this);
        ui->lineEdit_TopMargin->setValidator(validator1);
        ui->lineEdit_LeftMargin->setValidator(validator1);
        ui->lineEdit_ImageDensity->setValidator(validator1);

        ui->lineEdit_TopMargin->setText("0");
        ui->lineEdit_LeftMargin->setText("0");
        ui->lineEdit_ImageDensity->setText("0");
        ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        isLowHumidity = false;

        //ui->label_setting_error->hide();
        ui->label_TopMargin_error->hide();
        ui->label_LeftMargin_error->hide();
        ui->label_ImageDensity_error->hide();
        ui->lineEdit_TopMargin->installEventFilter(this);
        ui->lineEdit_LeftMargin->installEventFilter(this);
        ui->lineEdit_ImageDensity->installEventFilter(this);
    }
    else                //if get the value form device, set the value got;
    {
        ui->lineEdit_ImageDensity->setText(QString::number(config.ImageDensity));
        ui->lineEdit_LeftMargin->setText(QString::number(config.SideToSide));
        ui->lineEdit_TopMargin->setText(QString::number(config.LeadingEdge));

        isLowHumidity = (0 == config.LowHumidityMode ? false : true);
        if(isLowHumidity)
            ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        else
            ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");

    }
}

void SettingsStackedWidget::on_btAPOpen_clicked()
{
    isAPOpen = !isAPOpen;
    if(isAPOpen)
        ui->btAPOpen->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
    else
        ui->btAPOpen->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
}

void SettingsStackedWidget::on_btApply_AP_clicked()
{
    int ssid_len = ui->lineEdit_SSID_AP->text().length();
    int psd_len = ui->lineEdit_Password_AP->text().length();

    if(ssid_len < 1)
    {
        SettingWarming *msgWarm = new SettingWarming(0, tr("ResStr_Msg_9"));
        msgWarm->setWindowTitle("ResStr_Warning");
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint );
        msgWarm->exec();
        msgWarm->deleteLater();
    }
    else if(psd_len < 8)
    {
        SettingWarming *msgWarm  = new SettingWarming(0, tr("ResStr_Msg_3"));
        msgWarm->setWindowTitle("ResStr_Warning");
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint);
        msgWarm->exec();
        msgWarm->deleteLater();
    }
    else
    {
        if(!isLogn)                                     //if have not confirmed, show the authentication dialog;
        {
            emit cycleStart();
            AuthenticationDlg *dlg = new AuthenticationDlg(0, &isLogn);
            dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint );
            dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
            dlg->exec();
        }
        if(isLogn )//&& CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            QString ssid = ui->lineEdit_SSID_AP->text();
            memset(softap.ssid, 0, 32);
            memcpy(softap.ssid, ssid.left(32).toLatin1(), 32);
            QString psw = ui->lineEdit_Password_AP->text();
            memset(softap.pwd, 0, 64);
            memcpy(softap.pwd, psw.left(32).toLatin1(), 64);
            emit cycleStart();
            softap.wifiEnable = isAPOpen ? 7 : 1 ;
            QVariant data;
            data.setValue<cmdst_softap>(softap);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_WIFI_Set_SoftAp,data);     //setting softap
            LOGLOG("customer setting soft AP");
        }
        else
        {
            emit cycleStop();
        }
    }

}

//set ipv4 parameters
void SettingsStackedWidget::on_btApply_IPConfig_clicked()
{
    QStringList ipaddress = ui->lineEdit_IPAddressv4->text().split(".");
    QStringList ipgateway = ui->lineEdit_Gatewayv4->text().split(".");
    QStringList ipsubmask = ui->lineEdit_Submaskv4->text().split(".");

    if(ui->btDHCP->isChecked())             //using DHCP of ipv4
    {
        info_ipv4.IPMode = 0;
        info_ipv4.IPAddressMode = 3;
    }
    if(ui->btStatic->isChecked())           //setting static ipv4 address
    {
        info_ipv4.IPMode = 0;
        info_ipv4.IPAddressMode = 4;
        for(int i = 0; i<4; i++)
        {
            info_ipv4.IPAddress[i] = ipaddress.at(i).toInt();
            info_ipv4.GatewayAddress[i] = ipgateway.at(i).toInt();
            info_ipv4.SubnetMask[i] = ipsubmask.at(i).toInt();
        }
    }
    if(!isLogn)
    {
        emit cycleStart();
        AuthenticationDlg *dlg = new AuthenticationDlg(0, &isLogn);
        dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                            & ~Qt::WindowMinimizeButtonHint );
        dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
        dlg->exec();
    }
    if(isLogn )//&& CMD_STATUS_COMPLETE == device->get_cmdStatus())
    {
        QVariant data;
        data.setValue<net_info_st>(info_ipv4);
        gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_SetV4,data);
        emit cycleStart();
    }
    else
    {
        emit cycleStop();
    }

    ui->btApply_IPConfig->setFocus();
}

void SettingsStackedWidget::on_btDHCP_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit_IPAddressv4->setDisabled(true);
        ui->lineEdit_Gatewayv4->setDisabled(true);
        ui->lineEdit_Submaskv4->setDisabled(true);

//        ui->lineEdit_IPAddressv4->setStyleSheet("");
    }
    else
    {
        ui->lineEdit_IPAddressv4->setEnabled(true);
        ui->lineEdit_Gatewayv4->setEnabled(true);
        ui->lineEdit_Submaskv4->setEnabled(true);
    }
}

void SettingsStackedWidget::on_btStatic_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit_IPAddressv4->setStyleSheet("QLineEdit{\
                                                border:2px solid rgb(170, 170, 170);\
                                                border-radius:12px;}");
        ui->lineEdit_Gatewayv4->setStyleSheet("QLineEdit{\
                                              border:2px solid rgb(170, 170, 170);\
                                              border-radius:12px;}");
        ui->lineEdit_Submaskv4->setStyleSheet("QLineEdit{\
                                              border:2px solid rgb(170, 170, 170);\
                                              border-radius:12px;}");
    }
    else
    {
        ui->lineEdit_IPAddressv4->setStyleSheet("QLineEdit{\
                                                border:2px solid rgb(170, 170, 170);\
                                                border-radius:12px;\
                                                background-color: rgb(189, 189, 189);\
                                                color: rgb(255, 255, 255);}");
        ui->lineEdit_Gatewayv4->setStyleSheet("QLineEdit{\
                                              border:2px solid rgb(170, 170, 170);\
                                              border-radius:12px;\
                                              background-color: rgb(189, 189, 189);\
                                              color: rgb(255, 255, 255);}");
        ui->lineEdit_Submaskv4->setStyleSheet("QLineEdit{\
                                              border:2px solid rgb(170, 170, 170);\
                                              border-radius:12px;\
                                              background-color: rgb(189, 189, 189);\
                                              color: rgb(255, 255, 255);}");
    }
}

void SettingsStackedWidget::on_btIPv6Setting_clicked()
{
    settingIPv6Widget = new SettingForIPv6(this, &isLogn);
    settingIPv6Widget->setWindowTitle(tr("ResStr_Setting"));
    settingIPv6Widget->exec();
}

void SettingsStackedWidget::on_btIPv6Status_clicked()
{
    ipv6Status = new IPv6Status(this);
    ipv6Status->setWindowTitle(tr("ResStr_Status"));
    ipv6Status->exec();
}

/**************
 *to setting userconfig value
*****************/
void SettingsStackedWidget::on_btApply_userconfig_clicked()
{
    if(!isLogn )//&& CMD_STATUS_COMPLETE == device->get_cmdStatus())
    {
        emit cycleStart();
        AuthenticationDlg *dlg = new AuthenticationDlg(0, &isLogn);
        dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                            & ~Qt::WindowMinimizeButtonHint );
        dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
        dlg->exec();
    }
    if(isLogn)
    {
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())  {
            userconfig.LeadingEdge = ui->lineEdit_TopMargin->text().toInt();
            userconfig.SideToSide = ui->lineEdit_LeftMargin->text().toInt();
            userconfig.ImageDensity = ui->lineEdit_ImageDensity->text().toInt();
            userconfig.LowHumidityMode = (isLowHumidity ? 1 : 0);
            userconfig.PlateControlMode = (isConrtolM ? 0 : 2);
            userconfig.PrimaryCoolingMode = (isCoolingM ? 1 : 0);
            emit cycleStart();
            QVariant data;
            data.setValue<cmdst_userconfig>(userconfig);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_Set_UserConfig,data);
        //}
    }
    else
    {
        emit cycleStop();
    }
}

/**************
 *to set the power save time
*****************/
void SettingsStackedWidget::on_btApply_Timeout_clicked()
{
    if(!isLogn )//&& CMD_STATUS_COMPLETE == device->get_cmdStatus())
    {
        emit cycleStart();
        AuthenticationDlg *dlg = new AuthenticationDlg(0, &isLogn);
        dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                            & ~Qt::WindowMinimizeButtonHint );
        dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
        dlg->exec();
    }
    if(isLogn)
    {
        psavetm = ui->lineEdit_timeout->text().toInt();
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())  {
            QVariant data;
            data.setValue<cmdst_PSave_time>(psavetm);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_PSaveTime_Set,data);
            emit cycleStart();
        //}
    }
    else
    {
        emit cycleStop();
    }
}

void SettingsStackedWidget::on_lineEdit_TopMargin_textEdited(const QString &arg1)
{
    int val = arg1.toInt();
    if(val>2 || val<-2)
    {
//        ui->label_setting_error->setGeometry(47,ui->lineEdit_TopMargin->y()+20,201,35);
//        ui->label_setting_error->setText(tr("The valid range is from -2 to 2, please confirm and enter again"));
//        ui->label_setting_error->show();
        ui->label_TopMargin_error->show();
        ui->lineEdit_TopMargin->setStyleSheet("QLineEdit{border:2px solid red;}");
        ui->btTopAdd->setDisabled(true);
        ui->btTopReduce->setDisabled(true);
        ui->btApply_userconfig->setDisabled(true);
    }
    else
    {
        ui->lineEdit_TopMargin->setStyleSheet("QLineEdit{border:transparent;}");
        ui->label_TopMargin_error->hide();
        //ui->label_setting_error->hide();
        ui->btTopAdd->setEnabled(true);
        ui->btTopReduce->setEnabled(true);
        ui->btApply_userconfig->setEnabled(true);
    }
}

void SettingsStackedWidget::on_lineEdit_LeftMargin_textEdited(const QString &arg1)
{
    int val = arg1.toInt();
    if(val>6 || val<-6)
    {
//        ui->label_setting_error->setGeometry(47,ui->lineEdit_LeftMargin->y()+20,201,35);
//        ui->label_setting_error->setText(tr("The valid range is from -6 to 6, please confirm and enter again"));
//        ui->label_setting_error->show();
        ui->label_LeftMargin_error->show();
        ui->lineEdit_LeftMargin->setStyleSheet("QLineEdit{border:2px solid red;}");
        ui->btLeftAdd->setDisabled(true);
        ui->btLeftReduce->setDisabled(true);
        ui->btApply_userconfig->setDisabled(true);
    }
    else
    {
        ui->lineEdit_LeftMargin->setStyleSheet("QLineEdit{border:transparent;}");
        ui->label_LeftMargin_error->hide();
        //ui->label_setting_error->hide();
        ui->btLeftAdd->setEnabled(true);
        ui->btLeftReduce->setEnabled(true);
        ui->btApply_userconfig->setEnabled(true);
    }
}

void SettingsStackedWidget::on_lineEdit_ImageDensity_textEdited(const QString &arg1)
{
    int val = arg1.toInt();
    if(val>3 || val<-3)
    {
//        ui->label_setting_error->setGeometry(47,ui->lineEdit_ImageDensity->y()+20,201,35);
//        ui->label_setting_error->setText(tr("The valid range is from -3 to 3, please confirm and enter again"));
//        ui->label_setting_error->show();
        ui->label_ImageDensity_error->show();
        ui->lineEdit_ImageDensity->setStyleSheet("QLineEdit{border:2px solid red;}");
        ui->btDensityAdd->setDisabled(true);
        ui->btDensityReduce->setDisabled(true);
        ui->btApply_userconfig->setDisabled(true);
    }
    else
    {
        ui->lineEdit_ImageDensity->setStyleSheet("QLineEdit{border:transparent;}");
        ui->label_ImageDensity_error->hide();
        //ui->label_setting_error->hide();
        ui->btDensityAdd->setEnabled(true);
        ui->btDensityReduce->setEnabled(true);
        ui->btApply_userconfig->setEnabled(true);
    }
}

void SettingsStackedWidget::on_btDensityAdd_clicked()
{
    int val = ui->lineEdit_ImageDensity->text().toInt();
    if(val<3)
    {
        val++;
        ui->lineEdit_ImageDensity->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_btDensityReduce_clicked()
{
    int val = ui->lineEdit_ImageDensity->text().toInt();
    if(val>-3)
    {
        val--;
        ui->lineEdit_ImageDensity->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_btDrumReset_clicked()
{

}

void SettingsStackedWidget::on_btErrorClear_clicked()
{
    if(!isLogn)// && CMD_STATUS_COMPLETE == device->get_cmdStatus())
    {
        emit cycleStart();
        AuthenticationDlg *dlg = new AuthenticationDlg(0, &isLogn);
        dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                            & ~Qt::WindowMinimizeButtonHint );
        dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
        dlg->exec();
    }
    if(isLogn)
    {
        gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PRN_FusingScReset);
        emit cycleStart();
    }
    else
    {
        emit cycleStop();
    }
}

void SettingsStackedWidget::on_btLeftAdd_clicked()
{
    int val = ui->lineEdit_LeftMargin->text().toInt();
    if(val<6)
    {
        val++;
        ui->lineEdit_LeftMargin->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_btLeftReduce_clicked()
{
    int val = ui->lineEdit_LeftMargin->text().toInt();
    if(val>-6)
    {
        val--;
        ui->lineEdit_LeftMargin->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_btLowHumidity_clicked()
{
    isLowHumidity = !isLowHumidity;
    if(isLowHumidity)
        ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
    else
        ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
}

void SettingsStackedWidget::on_btTonerReset_clicked()
{

}

void SettingsStackedWidget::on_btTopAdd_clicked()
{
    int val = ui->lineEdit_TopMargin->text().toInt();
    if(val<2)
    {
        val++;
        ui->lineEdit_TopMargin->setText(QString::number(val));
    }
}

void SettingsStackedWidget::on_btTopReduce_clicked()
{
    int val = ui->lineEdit_TopMargin->text().toInt();
    if(val>-2)
    {
        val--;
        ui->lineEdit_TopMargin->setText(QString::number(val));
    }
}

/**************
 *to set the new password
*****************/
void SettingsStackedWidget::on_btApply_clicked()
{
    if(ui->lineEdit_newPassWord->text().isEmpty())
    {
        SettingWarming *msgWarm = new SettingWarming(0, tr("ResStr_The_new_password_can_not_be_empty_"));
        msgWarm->setWindowTitle("ResStr_Warning");
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint);
        msgWarm->exec();
        msgWarm->deleteLater();
    }
    else if(ui->lineEdit_confirm->text() != ui->lineEdit_newPassWord->text())
    {
        SettingWarming *msgWarm = new SettingWarming(0, tr("ResStr_The_passwords_you_entered__are_different__please_try_again_"));
        msgWarm->setWindowTitle("ResStr_Warning");
        msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint);
        msgWarm->exec();
        msgWarm->deleteLater();
    }
    else
    {
        if(!isLogn)// && CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            cycleStart();
            AuthenticationDlg *dlg = new AuthenticationDlg(0, &isLogn);
            dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                & ~Qt::WindowMinimizeButtonHint );
            dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
            dlg->exec();
        }
        if(isLogn)
        {
            cmdst_passwd passwd;
            QString text = ui->lineEdit_newPassWord->text();
            memcpy(passwd.passwd,text.left(32).toLatin1(),32);
            QVariant data;
            data.setValue<cmdst_passwd>(passwd);
            gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_PASSWD_set,data);
            emit cycleStart();
        }
        else
        {
            emit cycleStop();
        }
    }
}
