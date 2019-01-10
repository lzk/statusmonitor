#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "uinterface.h"
#include "membercenter/about.h"
#include "promptdialog.h"
#include "ids_string.h"
#include "animationdlg.h"
#include <qmenu.h>
#include <qdesktopservices.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LOGLOG("test!!!");
    ui->memberCenterWidget->setSW(ui->totalStackedWidget, ui->loginButton);

    selectState = "background-color: rgb(99, 99, 99);color:white;";
    unSelectState = "background-color: white;color:black;";

    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));
    connect(gUInterface, SIGNAL(setDeviceMsg(QString,int)),this, SLOT(setDeviceMsg(QString,int)));
//    connect(gUInterface, SIGNAL(updateStatus(QVariant)),this,SLOT(updateStatus(QVariant)));

    gUInterface->setCmd(UIConfig::CMD_GetPrinters,QString());
    gUInterface->setTimer(10);

//    qDebug()<<"horizontalLayout"<<ui->horizontalLayout->geometry();
//    qDebug()<<"refreshbtn"<<ui->refreshBtn->geometry();
    statusCycle = new BusyRefreshLabel(ui->deviceMsgWidget,true);
//    statusCycle->setGeometry(50,30,20,19);

    ui->cycleWidget->hide();
    ui->errorBtn->hide();
    errorStatus(true);

    cycle = new BusyRefreshLabel(ui->cycleWidget,false);
    cycle->setGeometry(385,280,80,80);

    connect(ui->tabStackedWidget,SIGNAL(cycleStartFromTab()),this,SLOT(startCycleAnimation()));
    connect(ui->tabStackedWidget,SIGNAL(cycleStopFromTab()),this,SLOT(stopCycleAnimation()));

    createSysTray();
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
//    restoreAction = new QAction(tr("显示(&R)"), this);
//    connect(restoreAction, SIGNAL(triggered(bool)), this, SLOT(showNormal()));
    quitAction = new QAction(tr("退出(&Q)"), this);
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
//    trayIconMenu->addAction(restoreAction);
//    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/Images/printer.ico"));

//    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

//rewrite the mousePressEvent
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        isPress = true;
        last = e->globalPos();
    }
    else
    {
        isPress = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(isPress)
    {
        int dx = e->globalX() - last.rx();
        int dy = e->globalY() - last.ry();
        last = e->globalPos();
        move(x()+dx, y()+dy);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    isPress = false;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    PromptDialog *pDialog = new PromptDialog(this);
    if(ui->tabStackedWidget->getScrollAreaImageStatus())//added by gavin 2016-04-07
    {
        pDialog->setDialogMsg(tr("ResStr_The_scanned_images_will_be_deleted_after_closing_the_VOP__Are_you_sure_to_close_the_VOP_"));
        pDialog->setDialogMsgAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    if (pDialog->exec() == QDialog::Accepted)
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::on_Copy_clicked()
{
    ui->Copy->setStyleSheet(selectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
    ui->Scan->setStyleSheet(unSelectState + "border-radius:0px");
    ui->Setting->setStyleSheet(unSelectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");

    ui->tabStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_Scan_clicked()
{
    ui->Copy->setStyleSheet(unSelectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
    ui->Scan->setStyleSheet(selectState + "border-radius:0px");
    ui->Setting->setStyleSheet(unSelectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");

    ui->tabStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_Setting_clicked()
{
    ui->Copy->setStyleSheet(unSelectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
    ui->Scan->setStyleSheet(unSelectState + "border-radius:0px");
    ui->Setting->setStyleSheet(selectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");

    ui->tabStackedWidget->setCurrentIndex(2);
    ui->tabStackedWidget->on_btn_WiFi_clicked();
}

void MainWindow::on_loginButton_clicked()
{
    ui->totalStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_CopyImgBtn_clicked()
{
    ui->Copy->setStyleSheet(selectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
    ui->Scan->setStyleSheet(unSelectState + "border-radius:0px");
    ui->Setting->setStyleSheet(unSelectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");

    ui->tabStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_ScanImgBtn_clicked()
{
    ui->Copy->setStyleSheet(unSelectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
    ui->Scan->setStyleSheet(selectState + "border-radius:0px");
    ui->Setting->setStyleSheet(unSelectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");

    ui->tabStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_SettingImgBtn_clicked()
{
    ui->Copy->setStyleSheet(unSelectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
    ui->Scan->setStyleSheet(unSelectState + "border-radius:0px");
    ui->Setting->setStyleSheet(selectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");

    ui->tabStackedWidget->setCurrentIndex(2);
    ui->tabStackedWidget->on_btn_WiFi_clicked();
}

void MainWindow::on_refreshBtn_clicked()
{

//    QPoint point = ui->refreshBtn->mapFromParent(ui->deviceMsgWidget->pos());
//    qDebug()<<"refresh"<<ui->refreshBtn->pos();
//    statusCycle->setGeometry(point.x(),point.y(),20,19);
    statusCycle->setGeometry(70,37,20,19);
//    qDebug()<<"status"<<statusCycle->pos();

    if(ui->tabStackedWidget->currentIndex() != 0)
    {
        on_Copy_clicked();
    }
    ui->refreshBtn->hide();
    gUInterface->setCmd(UIConfig::CMD_GetPrinters,QString());
    statusCycle->startAnimation(20);
}

void MainWindow::cmdResult(int cmd,int result ,QVariant data)
{
    switch(cmd){

    case UIConfig::CMD_GetPrinters:{
        if(!result){
            updatePrinter(data);
        }
        statusCycle->stopAnimation();
        ui->refreshBtn->show();
    }
        break;
    case UIConfig::CMD_GetStatus:{
        if(!result){
            LOGLOG("MainWindow")
            updateStatus(data);
        }else{//get status fail
            LOGLOG("get printer status fail!");
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::updatePrinter(const QVariant& data)
{
    printerInfos.clear();
    printerInfos = data.value<QList<PrinterInfo_struct> >();
    PrinterInfo_struct printerInfo;

    printers.clear();
    disconnect(ui->deviceNameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_deviceNameBox_currentIndexChanged(int)));
    ui->deviceNameBox->clear();
    connect(ui->deviceNameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_deviceNameBox_currentIndexChanged(int)));
    for(int i = 0 ;i < printerInfos.length() ;i++){
        printerInfo = printerInfos.at(i);
        printers << printerInfo.printer.name;

        ui->deviceNameBox->addItem(printerInfo.printer.name);
    }
    if(printers.isEmpty()){
        setcurrentPrinter(QString());
        return;
    }else if(printers.contains(current_printer)){
        ui->deviceNameBox->setCurrentIndex(printers.indexOf(current_printer));
    }else{
        setcurrentPrinter(printers.first());
    }

    on_Copy_clicked();

}

void MainWindow::setcurrentPrinter(const QString& str)
{
    current_printer = str;
    qDebug()<<"current printer:"<<current_printer;
    gUInterface->setcurrentPrinter(str);
    gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);
}

void MainWindow::updateToner(int c ,int m ,int y ,int k)
{
}


void MainWindow::updateStatus(QVariant data)
{
    LOGLOG("updateStatus");
    PrinterInfo_struct printerInfo = data.value<PrinterInfo_struct>();
    PrinterStatus_struct& status = printerInfo.status;
    LOGLOG("get status success:0x%02x" ,status.PrinterStatus);
    on_status_ch(status);
}

void MainWindow::updateOtherStatus(const QString&  ,const PrinterStatus_struct& )
{

}

void MainWindow::on_deviceNameBox_currentIndexChanged(int index)
{
    if(printers.at(index) != current_printer)
    {
        current_printer = printers.at(index);
        setcurrentPrinter(printers.at(index));
        if(ui->tabStackedWidget->currentIndex() != 0)
        {
            LOGLOG("on_deviceNameBox_currentIndexChanged");
            on_Copy_clicked();
        }
    }

}

void MainWindow::errorStatus(bool bIsErrorStatus)
{
    ui->tabStackedWidget->setCopyStackedWidgetCurrentIndex(bIsErrorStatus);
    ui->CopyImgBtn->setEnabled(!bIsErrorStatus);
    ui->ScanImgBtn->setEnabled(!bIsErrorStatus);
    ui->SettingImgBtn->setEnabled(!bIsErrorStatus);

    if(bIsErrorStatus)
    {
        ui->Copy->setStyleSheet("background-color: white;color:gray;border-top-right-radius:0px;border-bottom-right-radius:0px");
        ui->Scan->setStyleSheet("background-color: white;color:gray;border-radius:0px");
        ui->Setting->setStyleSheet("background-color: white;color:gray;border-top-left-radius:0px;border-bottom-left-radius:0px");
        ui->btCar->setStyleSheet("border-image: url(:/Images/shopCart_Disable.tif);");
        ui->btCar->setEnabled(false);
    }
    else
    { 
        ui->btCar->setStyleSheet("border-image: url(:/Images/shopCart_Normal.png);");
        ui->btCar->setEnabled(true);
    }

    ui->Copy->setEnabled(!bIsErrorStatus);
    ui->Scan->setEnabled(!bIsErrorStatus);
    ui->Setting->setEnabled(!bIsErrorStatus);
    ui->btCar->setEnabled(!bIsErrorStatus);

    ui->tabStackedWidget->set_setting_enabled(!bIsErrorStatus);

//    if(ui->deviceNameBox->isEnabled())
//    {
        ui->tabStackedWidget->set_scan_enabled(!bIsErrorStatus);//Added for default enable scan button by gavin 2016-04-14
        ui->tabStackedWidget->set_copy_enabled(!bIsErrorStatus);
//    }
//    ui->errorBtn->hide();

}

void MainWindow::set_Message_Background_Color(UIConfig::EnumStatus s)
{
    switch ( s )
    {
        case UIConfig::Ready                                   : ui->label_10->setStyleSheet("QLabel{color:Black;}"); break;
        case UIConfig::Printing                                : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::PowerSaving                             : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::WarmingUp                               : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::PrintCanceling                          : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::Processing                              : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::CopyScanning                            : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::CopyScanNextPage                        : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::CopyPrinting                            : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::CopyCanceling                           : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::IDCardMode                              : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::ScanScanning                            : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::ScanSending                             : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::ScanCanceling                           : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::ScannerBusy                             : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::TonerEnd1                               : ui->label_10->setStyleSheet("QLabel{color:Orange;}");; break;
        case UIConfig::TonerEnd2                               : ui->label_10->setStyleSheet("QLabel{color:Orange;}");; break;
        case UIConfig::TonerNearEnd                            : ui->label_10->setStyleSheet("QLabel{color:Orange;}");; break;
        case UIConfig::OPCNearEnd                              : ui->label_10->setStyleSheet("QLabel{color:Orange;}");; break;
        case UIConfig::OPCEnd                                  : ui->label_10->setStyleSheet("QLabel{color:Red;}");;    break;
        case UIConfig::ManualFeedRequired                      : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::InitializeJam                           : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::NofeedJam                               : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::JamAtRegistStayOn                       : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::JamAtExitNotReach                       : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::JamAtExitStayOn                         : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::CoverOpen                               : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::NoTonerCartridge                        : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::WasteTonerFull                          : ui->label_10->setStyleSheet("QLabel{color:Orange;}");; break;
        case UIConfig::PDLMemoryOver                          : ui->label_10->setStyleSheet("QLabel{color:Red;}");; break;
        case UIConfig::FWUpdate                                : ui->label_10->setStyleSheet("QLabel{color:Black;}"); ; break;
        case UIConfig::OverHeat                                : ui->label_10->setStyleSheet("QLabel{color:Orange;}");; break;
        case UIConfig::PolygomotorOnTimeoutError               : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::PolygomotorOffTimeoutError              : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::PolygomotorLockSignalError              : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::BeamSynchronizeError                    : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::BiasLeak                                : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::PlateActionError                        : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::MainmotorError                          : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::MainFanMotorEorror                      : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::JoinerThermistorError                   : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::JoinerReloadError                       : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::HighTemperatureErrorSoft                : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::HighTemperatureErrorHard                : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::JoinerFullHeaterError                   : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::Joiner3timesJamError                    : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::LowVoltageJoinerReloadError             : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::MotorThermistorError                    : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::EEPROMCommunicationError                : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::CTL_PRREQ_NSignalNoCome                 : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::SCAN_USB_Disconnect                     : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::SCAN_NET_Disconnect                     : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::ScanMotorError                          : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::SCAN_DRV_CALIB_FAIL                     : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::NetWirelessDongleCfgFail                : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        case UIConfig::DMAError                                : ui->label_10->setStyleSheet("QLabel{color:Red;}");   ; break;
        default:
                                                                  ui->label_10->setStyleSheet("QLabel{color:Black;}");;
                                                                  break;
    }
}

void MainWindow::on_status_ch(const PrinterStatus_struct& status)
{
    ui->label_10->setStyleSheet("QLabel{color:break;}");
    ui->mofenProgressBar->setValue(status.TonelStatusLevelK);
    errorStatus(false);

    UIConfig::StatusDisplayType displayStatus = UIConfig::GetStatusTypeForUI((UIConfig::EnumStatus)status.PrinterStatus);
    QString errMsg = UIConfig::getErrorMsg((UIConfig::EnumStatus)status.PrinterStatus,UIConfig::UnknowJob,0);

    ui->label_10->setText(errMsg);
    set_Message_Background_Color((UIConfig::EnumStatus)status.PrinterStatus);

    switch (displayStatus) {
    case UIConfig::Status_Ready:
        ui->label_6->setText(tr("ResStr_Ready"));
        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(53, 177, 20);}");


        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Green.png);");
        break;
    case UIConfig::Status_Sleep:
        ui->label_6->setText(tr("ResStr_Sleep"));
        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(53, 177, 20);}");
        break;
    case UIConfig::Status_Offline:
        ui->label_6->setText(tr("ResStr_Offline"));
        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(110, 110, 110);}");

        ui->tabStackedWidget->set_copy_enabled(false);
        ui->tabStackedWidget->set_setting_enabled(false);
        ui->tabStackedWidget->set_scan_enabled(false); //Added for disable scan button when offline by gavin 2016-04-14
        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Gray.png);");
        ui->mofenProgressBar->setValue(0);
        break;
    case UIConfig::Status_Busy:
        ui->label_6->setText(tr("ResStr_Busy"));
        ui->label_6->setStyleSheet("QLabel{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(53, 177, 20);}");

        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Green.png);");
        break;
    case UIConfig::Status_Error:
        ui->label_6->setText(tr("ResStr_Error"));
        ui->label_6->setStyleSheet("QLabel{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: red;}");       
        ui->errorBtn->show();

        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Red.png);");
        break;
    default:
        break;
    }
}

void MainWindow::setDeviceMsg(const QString& msg, int result)
{
    if(!result)
        ui->label_10->setStyleSheet("QLabel{color:red}");
    else
        ui->label_10->setStyleSheet("QLabel{color:black}");
    ui->label_10->setText(msg);
}

void MainWindow::startCycleAnimation()
{
    qDebug()<<"startCycleAnimation";
    ui->cycleWidget->show();
    cycle->startAnimation(20);
}

void MainWindow::stopCycleAnimation()
{
    qDebug()<<"stopCycleAnimation";
    ui->cycleWidget->hide();
    cycle->stopAnimation();
}

void MainWindow::on_errorBtn_clicked()
{
    bool enNextShow = false;
    AnimationDlg *aDialog = new AnimationDlg(this, 0xBD, &enNextShow);
    aDialog->setAttribute(Qt::WA_DeleteOnClose);
    if (aDialog->exec() == QDialog::Rejected)
    {
        return;
    }
}

void MainWindow::on_btCar_clicked()
{
    QDesktopServices::openUrl(QUrl("http://ibase.lenovoimage.com/buy_abc2.aspx"));
}
