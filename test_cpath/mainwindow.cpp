#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>
#include "uinterface.h"
#include <QDateTime>
#include <unistd.h>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
//#include <QMessageBox>

#define TEST 0
static const QString app_name = QString::fromUtf8("打印机状态监视器");
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(app_name);
//    setWindowIcon(QIcon(":/image/app_icon.png"));

    this->setFixedSize(700 ,480);

//    createSysTray();


    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

    gUInterface->setCmd(UIConfig::CMD_GetPrinters,QString());
//    gUInterface->setCmd(UIConfig::CMD_GetDefaultPrinter);
//    gUInterface->setTimer(6);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createSysTray()
{

//    minimizeAction = new QAction(tr("Mi&nimize"), this);
//    connect(minimizeAction, SIGNAL(triggered(bool)), this, SLOT(hide()));
////    connect(minimizeAction, SIGNAL(triggered(bool)), this, SLOT(showMinimized()));

//    maximizeAction = new QAction(tr("Ma&ximize"), this);
//    connect(maximizeAction, SIGNAL(triggered(bool)), this, SLOT(showMaximized()));

//    restoreAction = new QAction(tr("&Restore"), this);
    restoreAction = new QAction(tr("显示(&R)"), this);
    connect(restoreAction, SIGNAL(triggered(bool)), this, SLOT(showNormal()));
    quitAction = new QAction(tr("退出(&Q)"), this);
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/image/app_icon.png"));

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
//        showMessage();
        break;
    default:
        ;
    }
}

void MainWindow::messageClicked()
{
    showNormal();
}

//void MainWindow::closeEvent(QCloseEvent *event)
//{
//#ifdef Q_OS_OSX
//    if (!event->spontaneous() || !isVisible()) {
//        return;
//    }
//#endif
//    if (trayIcon->isVisible()) {
////        QMessageBox::information(this, tr("Systray"),
////                                 tr("The program will keep running in the "
////                                    "system tray. To terminate the program, "
////                                    "choose <b>Quit</b> in the context menu "
////                                    "of the system tray entry."));
//        hide();
//        event->ignore();
//    }
//}

void MainWindow::cmdResult(int cmd,int result ,QVariant data)
{
    switch(cmd){
    case UIConfig::CMD_GetDefaultPrinter:{
        if(!result){
            QString default_printer = data.toString();
            setcurrentPrinter(default_printer);
        }
    }
        break;
    case UIConfig::CMD_GetStatus:{
        PrinterInfo_struct printerInfo = data.value<PrinterInfo_struct>();
        PrinterStatus_struct& status = printerInfo.status;
        LOGLOG("update status of %s" ,printerInfo.printer.name);
        if(!result){
            LOGLOG("get status success:0x%02x" ,status.PrinterStatus);
        }else{//get status fail
            LOGLOG("get printer status fail!");
            memset(&status ,-1 ,sizeof(status));
//                status.PrinterStatus = -1;
        }
        if(!current_printer.compare(printerInfo.printer.name)){
            updateToner(status.TonelStatusLevelC ,status.TonelStatusLevelM ,status.TonelStatusLevelY ,status.TonelStatusLevelK);
            updateStatus(status);
        }
        updateOtherStatus(printerInfo.printer.name ,status);
    }
        break;

    case UIConfig::CMD_GetPrinters:{
        if(!result){
            updatePrinter(data);
        }
    }
        break;

    case UIConfig::LS_CMD_GetCopy:{
        copycmdset device_para = data.value<copycmdset>();
        LOGLOG("lshell get copy num:%d" ,device_para.copyNum);
    }
        break;
    case UIConfig::LS_CMD_COPY:
    case UIConfig::LS_CMD_WIFI_apply:
    case UIConfig::LS_CMD_WIFI_apply_noread:
        break;
    case UIConfig::LS_CMD_WIFI_get:{
        cmdst_wifi_get device_para = data.value<cmdst_wifi_get>();
        LOGLOG("lshell get wifi ssid:%s" ,device_para.ssid);
    }
        break;
    case UIConfig::LS_CMD_WIFI_getAplist:{
        cmdst_aplist_get device_para = data.value<cmdst_aplist_get>();
        LOGLOG("lshell get aplist ssid:");
        for(int i = 0 ;i < 10 ;i++){
            LOGLOG("\t\t%d:%s" ,i+1 ,device_para.aplist[i].ssid);
        }
    }
        break;
    case UIConfig::LS_CMD_PASSWD_set:
        break;
    case UIConfig::LS_CMD_PASSWD_get:{
        cmdst_passwd device_para = data.value<cmdst_passwd>();
        LOGLOG("lshell get password:%s" ,device_para.passwd);
    }
        break;
    case UIConfig::LS_CMD_PASSWD_confirm:
        break;
    case UIConfig::LS_CMD_WIFI_GetWifiStatus:{
        cmdst_wifi_status device_para = data.value<cmdst_wifi_status>();
        LOGLOG("lshell get wifi status:%d" ,device_para);
    }
        break;
    case UIConfig::LS_CMD_PRN_TonerEnd_Get:{
        cmdst_tonerEnd device_para = data.value<cmdst_tonerEnd>();
        LOGLOG("lshell get toner end:%d" ,device_para);
    }
        break;
    case UIConfig::LS_CMD_PRN_TonerEnd_Set:
        break;
    case UIConfig::LS_CMD_PRN_PSaveTime_Get:{
        cmdst_PSave_time device_para = data.value<cmdst_PSave_time>();
        LOGLOG("lshell get save time:%d" ,device_para);
    }
        break;
    case UIConfig::LS_CMD_PRN_PSaveTime_Set:
        break;
    case UIConfig::LS_CMD_PRN_PowerOff_Get:{
        cmdst_powerOff_time device_para = data.value<cmdst_powerOff_time>();
        LOGLOG("lshell get power off time:%d" ,device_para);
    }
        break;
    case UIConfig::LS_CMD_PRN_PowerOff_Set:
        break;
    case UIConfig::LS_CMD_NET_GetV4:{
        net_info_st device_para = data.value<net_info_st>();
        LOGLOG("lshell get ipv4 :%d.%d.%d.%d" ,device_para.IPAddress[0],device_para.IPAddress[1],device_para.IPAddress[2],device_para.IPAddress[3]);
    }
        break;
    case UIConfig::LS_CMD_NET_SetV4:
        break;
    case UIConfig::LS_CMD_NET_GetV6:{
        net_ipv6_st device_para = data.value<net_ipv6_st>();
        LOGLOG("lshell get ipv6 :%s" ,device_para.ManualAddress);
    }
        break;
    case UIConfig::LS_CMD_NET_SetV6:
        break;
    case UIConfig::LS_CMD_PRN_Set_UserConfig:
        break;
    case UIConfig::LS_CMD_PRN_Get_UserConfig:{
        cmdst_userconfig device_para = data.value<cmdst_userconfig>();
        LOGLOG("lshell get user config image density:%d" ,device_para.ImageDensity);
    }
        break;
    case UIConfig::LS_CMD_WIFI_Set_SoftAp:
        break;
    case UIConfig::LS_CMD_WIFI_Get_SoftAp:{
        cmdst_softap device_para = data.value<cmdst_softap>();
        LOGLOG("lshell get softap :%s" ,device_para.ssid);
    }
        break;
    case UIConfig::LS_CMD_PRN_FusingScReset:{
        cmdst_fusingScReset device_para = data.value<cmdst_fusingScReset>();
        LOGLOG("lshell get fusing sc reset:%d" ,device_para);
    }
        break;
    case UIConfig::CMD_Scan:
        LOGLOG("scan result:%d" ,result);
    default:
        break;
    }
}

void MainWindow::setcurrentPrinter(const QString& str)
{
    current_printer = str;
    gUInterface->setcurrentPrinter(str);
    setWindowTitle(app_name + " - " + str);
    gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);
}

void MainWindow::on_listWidget_printers_itemDoubleClicked(QListWidgetItem *item)
{
    setcurrentPrinter(item->text());
}

void MainWindow::updateToner(int c ,int m ,int y ,int k)
{
    (void)c;
    (void)m;
    (void)y;
    (void)k;
}

QString MainWindow::get_Status_string(const PrinterStatus_struct& status)
{
    QString str_status;
    ErrorInfo_struct ei = getErrorInfo(status.ErrorCodeGroup, status.ErrorCodeID, status.PaperType, status.PaperSize);
    int ps = status.PrinterStatus;
    if ((IsStatusError(ps) && !IsStatusVirtual(ps) && ps != PS_ERROR_NOT_AVAILABLE && ps != PS_ERROR_NOT_SUPPORT) || ps == PS_TONER_LOW) {
        if(!ei.error || !ei.errorString){
            return IDS_STRUnknow;
        }
        str_status = QString() + ei.errorString->title + "(" + ei.error->code + ")";
        return str_status;
    }

    switch (ps) {
    case 0://Ready
        if(status.ErrorCodeGroup == 0x05 && ((status.ErrorCodeID == 43) || (status.ErrorCodeID == 66) || (status.ErrorCodeID == 70))){
            if(!ei.error || !ei.errorString){
                str_status = IDS_STRUnknow;
            }else{
                str_status = QString() + ei.errorString->title + "(" + ei.error->code + ")";
            }
        }else{
            str_status = IDS_STATUSReady;
        }
        break;
    case 1://Printing
        str_status = IDS_STATUSPrinting;
        break;
    case 2://Power Saving
        str_status = IDS_STATUSPowerSaving;
        break;
    case 3://Warmming Up
        str_status = IDS_STATUSWarningUp;
        break;
    case 4://Pending Deletion
        str_status = IDS_STRPendingDeletion;
        break;
    case 5://Paused
        str_status = IDS_STATUSPaused;
        break;
    case 6://Waiting
        str_status = IDS_STATUSWaiting;
        break;
    case 7://Processing
        str_status = IDS_STRProcessing;
        break;
    case 8://Busy
        str_status = IDS_STATUSBusy;
        break;
    case 0x80://Offline
        str_status = IDS_STATUSOffline;
        break;
    case 0x81://Toner low
        str_status = IDS_STATUSTonerLow;
        break;
    case 0x82://Initializing
        str_status = IDS_STATUSInitializing;
        break;
    case PS_MANUAL_FEED_REQUIRED:
        str_status = IDS_STATUSManualFeedRequired;
        break;
    case 0xc3: //Out of paper
        str_status = IDS_STATUSOutOfPaper;
        break;
    case 0xc5: //No toner
        str_status = IDS_STATUSNoToner;
        break;
    case 0xc1: //Door open
    case PS_ERROR_ADF_COVER_OPEN:
        str_status = IDS_STATUSDoorOpen;
        break;
    case 0xc7: //Not available
        str_status = IDS_STATUSNotAV;
        break;
    case 0xc4: //Paper problem
        str_status = IDS_STATUSPaperProblem;
        break;
    case 0xc6: //Page error
        str_status = IDS_STATUSPageError;
        break;
    case 0xc0: //Paper Jam
    case PS_ERROR_ADF_PAPER_JAM:
        str_status = IDS_STATUSPaperJam;
        break;
    case 0xc2: //Out of memory
        str_status = IDS_STATUSOutOfM;
        break;
    case 0xC9: //Not Supported
        str_status = IDS_STRUnknow;
//        str_status = IDS_STATUSNotSupported;
        break;
    case 0xca: //User intervention required
        str_status = IDS_STATUSUserIR;
        break;
    case 0xCF: //Error
        str_status = IDS_STATUSError;
        break;
    case 0xCE: //Power Off
        str_status = IDS_STRPOWEROFF;
        break;

    case PS_UNKNOWN:
    default:
        str_status = IDS_STRUnknow;
        break;
    }
   return str_status;
}

void MainWindow::updateStatus(const PrinterStatus_struct& status)
{
    QString text;
    int i;
    QString toner_text;

    int currStatus = status.PrinterStatus;
    bool bShowLowTonerAlert = !!status.LowTonerAlert; // BMS#51330
    if (bShowLowTonerAlert && !IsStatusUnknownToner(currStatus) && StatusMonitor::AnyTonerReachLevel1(status) && !StatusMonitor::IsNonDellTonerMode(status)) {
        if (StatusMonitor::OnlyColorTonerEmpty(status)) {
            if (IsStatusPrinting(currStatus))
                toner_text = IDS_TONER_NEED_REPLACE;
            else
                toner_text = QString();
        }
        else {
            toner_text = IDS_TONER_IS_GETTING_LOW;
        }
    }
    else {
        toner_text = QString();
    }


    bool showDetail;
    const PrinterStatus_struct *ps = &status;
    if (IsStatusVirtual(ps->PrinterStatus) || IsStatusTrans(ps->PrinterStatus) || ps->ErrorCodeGroup == 0 || ps->ErrorCodeID == 0 || ps->PrinterStatus == PS_ERROR_NOT_SUPPORT) {
        showDetail = false;
    }
    else {
        if (IsStatusNormal(ps->PrinterStatus))
            showDetail = false;
        else {
            // Status is abnormal , then check if there are LTC code from FW
            if (ps->ErrorCodeGroup == 0 || ps->ErrorCodeID == 0)
                showDetail = false;
            else {
                showDetail = true;
            }
        }
    }

    text = "<html><head/><body>";
    if(!showDetail){
        text += QString() + "<p>&nbsp;&nbsp;&nbsp;&nbsp;" + get_Status_string(status) + "</p>";
    }else{
        ErrorInfo_struct ei = getErrorInfo(status.ErrorCodeGroup ,status.ErrorCodeID ,status.PaperType ,status.PaperSize);
        if(!ei.error || !ei.errorString){
            text += QString() + "<p>&nbsp;&nbsp;&nbsp;&nbsp;" + get_Status_string(status) + "</p>";
            goto CODE_ERROR;
        }

        text += QString() + "<p>&nbsp;&nbsp;&nbsp;&nbsp;" + ei.errorString->title + "</p>";
        text += "<br/>";
        text += QString() + "<p>" + ei.error->code + "</p>";
        for(i = 0 ;i < ei.errorString->lines ;i++){
            text += "<br/>";
            switch (i) {
            case 0:
                text += QString() + "<p>" + ei.errorString->line0 + "</p>";
                break;
            case 1:
                text += QString() + "<p>" + ei.errorString->line1 + "</p>";
                break;
            case 2:
                text += QString() + "<p>" + ei.errorString->line2 + "</p>";
                break;
            case 3:
                text += QString() + "<p>" + ei.errorString->line3 + "</p>";
                break;
            case 4:
                text += QString() + "<p>" + ei.errorString->line4 + "</p>";
                break;
            default:
                break;
            }
        }
    }
    CODE_ERROR:
    text += "</body></html>";
    ui->label_status->setText(text);
}

void MainWindow::updatePrinter(const QVariant& data)
{
    printerInfos = data.value<QList<PrinterInfo_struct> >();
    PrinterInfo_struct printerInfo;

    printers.clear();
    ui->listWidget_printers->clear();
    for(int i = 0 ;i < printerInfos.length() ;i++){
        printerInfo = printerInfos.at(i);
        printers << printerInfo.printer.name;

        ui->listWidget_printers->addItem(printerInfo.printer.name);
    }
    if(printers.contains(current_printer)){
        ui->listWidget_printers->setCurrentRow(printers.indexOf(current_printer));
    }else{
        setcurrentPrinter(printers.first());
    }
}

void MainWindow::updateOtherStatus(const QString&  ,const PrinterStatus_struct& )
{

}



void MainWindow::on_btn_wifi_get_clicked()
{
    gUInterface->setCmd(UIConfig::LS_CMD_WIFI_get ,current_printer);
}

#include "lshell.h"
void MainWindow::on_btn_copy_clicked()
{
//    copycmdset device_data;
//    LShell::copy_get_defaultPara(&device_data);
//    QVariant value;
//    value.setValue<copycmdset>(device_data);
//    gUInterface->setCmd(UIConfig::LS_CMD_COPY ,current_printer ,value);

    ScanSettings scan_settings;
    UiSettings* settings = &scan_settings.settings;
    settings->brightness = 50;
    settings->contrast = 50;
    settings->colorModel = Color;//Grayscale;// Color;
    settings->scan_dpi = Scan_300DPI;
    settings->scan_doctype = T_Photo;
    settings->scan_size = Scan_A4;
    QVariant value;
    value.setValue<ScanSettings>(scan_settings);
    gUInterface->setCmd(UIConfig::CMD_Scan ,current_printer ,value);
}

void MainWindow::on_btn_get_aplist_clicked()
{
    gUInterface->setCmd(UIConfig::LS_CMD_WIFI_getAplist ,current_printer);
}

void MainWindow::on_btn_get_ipv4_clicked()
{
    gUInterface->setCmd(UIConfig::LS_CMD_NET_GetV4 ,current_printer);
}

void MainWindow::on_btn_password_get_clicked()
{
    gUInterface->setCmd(UIConfig::LS_CMD_PASSWD_get ,current_printer);
}

void MainWindow::on_btn_password_set_clicked()
{
    cmdst_passwd device_data;
    sprintf(device_data.passwd ,"888888");
    QVariant value;
    value.setValue<cmdst_passwd>(device_data);
    gUInterface->setCmd(UIConfig::LS_CMD_PASSWD_set ,current_printer ,value);

}
