#include "settingforipv6.h"
#include "ui_settingforipv6.h"
#include "authenticationdlg.h"
#include "settingwarming.h"

#define RETRYTIMES 3;

SettingForIPv6::SettingForIPv6(QWidget *parent, bool *_isLogin) :
    QDialog(parent),
    ui(new Ui::SettingForIPv6),
    islogin(_isLogin)
{
    ui->setupUi(this);
    ui->checkBox_isDHCP->setChecked(false);
    ui->checkBox_isManualAddress->setChecked(false);

    ui->lineEdit_address->setDisabled(true);
    ui->lineEdit_address->installEventFilter(this);
    ui->lineEdit_SubMask->setDisabled(true);
    ui->lineEdit_GateAddress->installEventFilter(this);
    ui->lineEdit_GateAddress->setDisabled(true);
    ui->lineEdit_SubMask->installEventFilter(this);

//^([0-9a-f]{1,4}:){7}[0-9a-f]{1,4}$
    QRegExp rx1("[0-9a-fA-F:]{2,39}");
    QRegExpValidator *validator1 = new QRegExpValidator(rx1,this);
    ui->lineEdit_address->setValidator(validator1);
    ui->lineEdit_GateAddress->setValidator(validator1);

    QRegExp rx2("[0-9]{1,3}$");
    QRegExpValidator *validator2 = new QRegExpValidator(rx2, this);
    ui->lineEdit_SubMask->setValidator(validator2);

    ui->label_ipaddr_error->hide();
    ui->label_submask_error->hide();
    ui->label_gateway_error->hide();

    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)), this ,SLOT(cmdResult(int,int,QVariant)));
    cycle = new BusyRefreshLabel(this, true);
    cycle->setGeometry(QRect(260,90,50,50));
    cycle->startAnimation(20);
    gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_GetV6);
    isDoingCMD = false;
    times = 0;
}

SettingForIPv6::~SettingForIPv6()
{
    delete ui;
}

void SettingForIPv6::on_pushButton_clicked()
{
    close();
}

void SettingForIPv6::on_btCancel_clicked()
{
    close();
}

void SettingForIPv6::on_checkBox_isManualAddress_toggled(bool checked)
{
    if(checked)
    {
        ui->lineEdit_address->setEnabled(true);
        ui->lineEdit_address->setStyleSheet("QLineEdit{\
                                            border: 2px solid rgb(170, 170, 170);\
                                            border-radius:12;}");
        ui->lineEdit_SubMask->setEnabled(true);
        ui->lineEdit_SubMask->setStyleSheet("QLineEdit{\
                                            border: 2px solid rgb(170, 170, 170);\
                                            border-radius:12;}");
        ui->lineEdit_GateAddress->setEnabled(true);
        ui->lineEdit_GateAddress->setStyleSheet("QLineEdit{\
                                            border: 2px solid rgb(170, 170, 170);\
                                            border-radius:12;}");
    }
    else
    {
        ui->lineEdit_address->setDisabled(true);
        ui->lineEdit_address->setStyleSheet("QLineEdit{\
                                            border: 2px solid rgb(170, 170, 170);\
                                            border-radius:12;\
                                            background-color: rgb(189, 189, 189);\
                                            color: rgb(255, 255, 255);}");
        ui->lineEdit_SubMask->setDisabled(true);
        ui->lineEdit_SubMask->setStyleSheet("QLineEdit{\
                                            border: 2px solid rgb(170, 170, 170);\
                                            border-radius:12;\
                                            background-color: rgb(189, 189, 189);\
                                            color: rgb(255, 255, 255);}");
        ui->lineEdit_GateAddress->setDisabled(true);
        ui->lineEdit_GateAddress->setStyleSheet("QLineEdit{\
                                            border: 2px solid rgb(170, 170, 170);\
                                            border-radius:12;\
                                            background-color: rgb(189, 189, 189);\
                                            color: rgb(255, 255, 255);}");
    }
}

void SettingForIPv6::on_btApply_clicked()
{
    //check data is correct
    if(ui->checkBox_isManualAddress->isChecked() == true)
    {
        //check data is correct
        QRegExp reg_address("^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}(:[0-9a-fA-F]{1,4}){0,5}$");
        if(!reg_address.exactMatch(ui->lineEdit_address->text()))
        {
            ui->lineEdit_address->setFocus();
            SettingWarming *msgWarm  = new SettingWarming(this, tr("ResStr_The_entered_IP_address__is_wrong__please_confirm_and_enter_again_"));
            msgWarm->setWindowTitle(tr("ResStr_Warning"));
            msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                    & ~Qt::WindowMinimizeButtonHint);
            msgWarm->exec();

            return;
        }
        QRegExp reg_gateAddress("^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}(:[0-9a-fA-F]{1,4}){0,5}$");
        if(!reg_gateAddress.exactMatch(ui->lineEdit_GateAddress->text()))
        {
            ui->lineEdit_GateAddress->setFocus();
            SettingWarming *msgWarm  = new SettingWarming(this, tr("ResStr_The_entered_Gateway_is_wrong__please_confirm_and_enter_again_"));
            msgWarm->setWindowTitle(tr("ResStr_Warning"));
            msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                    & ~Qt::WindowMinimizeButtonHint);
            msgWarm->exec();

            return;
        }
        if(ui->lineEdit_SubMask->text().toInt(0,10) >= 128)
        {
            ui->lineEdit_SubMask->setFocus();
            SettingWarming *msgWarm  = new SettingWarming(this, tr("ResStr_The_subnet_mask_input_error__please_input_again_after_confirmation"));
            msgWarm->setWindowTitle(tr("ResStr_Warning"));
            msgWarm->setWindowFlags(msgWarm->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                                    & ~Qt::WindowMinimizeButtonHint);
            msgWarm->exec();

            return;
        }

    }

    if(!(*islogin ))
    {
        AuthenticationDlg *dlg = new AuthenticationDlg(this, islogin);
        dlg->setWindowFlags(dlg->windowFlags() & ~Qt::WindowMaximizeButtonHint \
                            & ~Qt::WindowMinimizeButtonHint );
        dlg->setWindowTitle(tr("ResStr_Identity_Authentication"));
        dlg->exec();
    }
    if(*islogin)
    {
        info_ipv6.DHCPv6 = ui->checkBox_isDHCP->isChecked() ? 1 : 0;
        info_ipv6.UseManualAddress = ui->checkBox_isManualAddress->isChecked() ? 1 : 0 ;
        if(ui->lineEdit_address->text().length() > 40)
            memcpy(info_ipv6.ManualAddress, ui->lineEdit_address->text().left(40).toLatin1(), 40);
        else
        {
            memset(info_ipv6.ManualAddress, 0, 40);
            memcpy(info_ipv6.ManualAddress, ui->lineEdit_address->text().left(40).toLatin1(), 40);
        }
        info_ipv6.ManualMask = ui->lineEdit_SubMask->text().toInt();

        if(ui->lineEdit_GateAddress->text().length() > 40)
            memcpy(info_ipv6.IPv6ManualGatewayAddress, ui->lineEdit_GateAddress->text().left(40).toLatin1(), 40);
        else
        {
            memset(info_ipv6.IPv6ManualGatewayAddress, 0, 40);
            memcpy(info_ipv6.IPv6ManualGatewayAddress, ui->lineEdit_GateAddress->text().left(40).toLatin1(), 40);
        }

        cycle->startAnimation(20);
        QVariant data;
        data.setValue<net_ipv6_st>(info_ipv6);
        gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_SetV6,data);
    }
}

//void SettingForIPv6::on_lineEdit_address_textEdited(const QString &arg1)
//{
//    QRegExp reg("^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}(:[0-9a-fA-F]{1,4}){0,5}$");
//    if(!reg.exactMatch(arg1))
//    {
//         ui->label_ipaddr_error->show();
//         ui->lineEdit_address->setStyleSheet("QLineEdit{\
//                                                 border:2px solid red;\
//                                                 border-radius:12px;}");
//        ui->btApply->setDisabled(true);
//    }
//    else
//    {
//        ui->label_ipaddr_error->hide();
//        ui->lineEdit_address->setStyleSheet("QLineEdit{\
//                                                border:2px solid rgb(170, 170, 170);\
//                                                border-radius:12px;}");
//        ui->btApply->setEnabled(true);
//    }
//}

//void SettingForIPv6::on_lineEdit_GateAddress_textEdited(const QString &arg1)
//{
//    QRegExp reg("^([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}(:[0-9a-fA-F]{1,4}){0,5}$");
//    if(!reg.exactMatch(arg1))
//    {
//         ui->label_gateway_error->show();
//         ui->lineEdit_GateAddress->setStyleSheet("QLineEdit{\
//                                                 border:2px solid red;\
//                                                 border-radius:12px;}");
//        ui->btApply->setDisabled(true);
//    }
//    else
//    {
//        ui->label_gateway_error->hide();
//        ui->lineEdit_GateAddress->setStyleSheet("QLineEdit{\
//                                                border:2px solid rgb(170, 170, 170);\
//                                                border-radius:12px;}");
//        ui->btApply->setEnabled(true);
//    }
//}

//void SettingForIPv6::on_lineEdit_SubMask_textEdited(const QString &arg1)
//{
////    QRegExp reg("^[1-3]{0,1}\\d$");
////    if(!reg.exactMatch(arg1))
//    if(arg1.toInt() >= 128)
//    {
//         ui->label_submask_error->show();
//         ui->lineEdit_SubMask->setStyleSheet("QLineEdit{\
//                                                 border:2px solid red;\
//                                                 border-radius:12px;}");
//        ui->btApply->setDisabled(true);
//    }
//    else
//    {
//        ui->label_submask_error->hide();
//        ui->lineEdit_SubMask->setStyleSheet("QLineEdit{\
//                                                border:2px solid rgb(170, 170, 170);\
//                                                border-radius:12px;}");
//        ui->btApply->setEnabled(true);
//    }
//}

bool SettingForIPv6::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->lineEdit_address)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(!(ui->label_ipaddr_error->isHidden()))
            {
                ui->lineEdit_address->setText("::");
                ui->label_ipaddr_error->hide();
                ui->lineEdit_address->setStyleSheet("QLineEdit{\
                                                    border:2px solid rgb(170, 170, 170);\
                                                    border-radius:12px;}");
                ui->btApply->setEnabled(true);
            }
        }
    }
    if(watched == ui->lineEdit_GateAddress)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(!(ui->label_gateway_error->isHidden()))
            {
                ui->lineEdit_GateAddress->setText("::");
                ui->label_gateway_error->hide();
                ui->lineEdit_GateAddress->setStyleSheet("QLineEdit{\
                                                              border:2px solid rgb(170, 170, 170);\
                                                              border-radius:12px;}");
                ui->btApply->setEnabled(true);
            }
        }
    }
    if(watched == ui->lineEdit_SubMask)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(!(ui->label_submask_error->isHidden()))
            {
                ui->lineEdit_SubMask->setText("0");
                ui->label_submask_error->hide();
                ui->lineEdit_SubMask->setStyleSheet("QLineEdit{\
                                                              border:2px solid rgb(170, 170, 170);\
                                                              border-radius:12px;}");
                ui->btApply->setEnabled(true);
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void SettingForIPv6::showInfo(net_ipv6_st info)
{
    if(info.DHCPv6 == 0 )
        ui->checkBox_isDHCP->setChecked(false);
    else
        ui->checkBox_isDHCP->setChecked(true);
    if(info.UseManualAddress == 0)
        ui->checkBox_isManualAddress->setChecked(false);
    else
        ui->checkBox_isManualAddress->setChecked(true);
    ui->lineEdit_address->setText(QString(info.ManualAddress));
    ui->lineEdit_SubMask->setText(QString::number(info.ManualMask));
    ui->lineEdit_GateAddress->setText(QString(info.IPv6ManualGatewayAddress));
}

void SettingForIPv6::cmdResult(int cmd,int result,QVariant data)
{
    switch(cmd)
    {
    case UIConfig::LS_CMD_NET_GetV6:
        if(!result)
        {
            info_ipv6 = data.value<net_ipv6_st>();
            showInfo(info_ipv6);
            LOGLOG("get ipv6 information");
            isDoingCMD = false;
            times = 0;
        }
        else{
            if(!isDoingCMD){
                isDoingCMD = true;
                times = RETRYTIMES;
            }
            if(times > 0){
                times--;
                gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_GetV6);
            }
            else {
                isDoingCMD = false;
            }
        }
        if(!isDoingCMD)
            cycle->stopAnimation();
        break;
    case UIConfig::LS_CMD_NET_SetV6:
        if(!result)
        {
            isDoingCMD = false;
            times = 0;
            close();
            LOGLOG("set ipv6 information");
        }
        else{
            if(!isDoingCMD){
                isDoingCMD = true;
                times = RETRYTIMES;
            }
            if(times > 0){
                times--;

                QVariant data;
                data.setValue(info_ipv6);
                gUInterface->setCurrentPrinterCmd(UIConfig::LS_CMD_NET_SetV6,data);
            }
            else {
                isDoingCMD = false;
            }
        }
        if(!isDoingCMD)
        {
            QString deviceMsg;
            if(!result)
            {
                deviceMsg = tr("ResStr_Msg_1");
            }
            else
            {
                deviceMsg = tr("ResStr_Setting_Fail");
            }
            cycle->stopAnimation();
            gUInterface->setDeviceMsgFrmUI(deviceMsg,0);
        }
        break;
    default: break;
    }
}
