#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "uinterface.h"
#include "membercenter/about.h"
#include "promptdialog.h"
#include "ids_string.h"
#include "animationdlg.h"
#include <qmenu.h>
#include <qdesktopservices.h>
#include <qlistview.h>
#include <qsettings.h>
#include "membercenter/experiencepro.h"

//#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(" ");
//    LOGLOG("test!!!");
    ui->memberCenterWidget->setSW(ui->totalStackedWidget, ui->loginButton);

    selectState = "background-color: rgb(99, 99, 99);color:white;";
    unSelectState = "background-color: white;color:black;";

    QListView *listView = new QListView(ui->deviceNameBox);
    listView->setStyleSheet("QListView{border-color:black;border-width:2px;border-radius:0px;}");
    listView->setStyleSheet("QListView::item:!selected{background-color:white;color:black;}");
    listView->setStyleSheet("QListView::item:selected:!active{background-color:gray;color:black;}");
    listView->setStyleSheet("QListView::item:selected:active{background-color:gray;color:white;}");
    ui->deviceNameBox->setView(listView);


    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));
    connect(gUInterface, SIGNAL(setDeviceMsg(QString,int)),this, SLOT(setDeviceMsg(QString,int)));
//    connect(gUInterface, SIGNAL(updateStatus(QVariant)),this,SLOT(updateStatus(QVariant)));
    connect(gUInterface,SIGNAL(startScan()),this, SLOT(startScan()));
    connect(gUInterface,SIGNAL(stopScan()),this,SLOT(stopScan()));
    connect(gUInterface,SIGNAL(signalEnabledCycleAnimation(bool)),this,SLOT(enableCycleAnimation(bool)));

    ui->cycleWidget->hide();
    ui->errorBtn->hide();
#ifndef DEBUG
    gUInterface->setCmd(UIConfig::CMD_GetPrinters,QString());
    isOfflineStart = true;
    enabledScanCopy = true;
    enableTroubleshootingPage(true);
    isStartCopy = false;

#else
    qDebug()<<"Status_Ready";
    printers << QString("Lenovo Pro");
    printers << QString("Lenovo Pro D");
    printers << QString("Lenovo Pro L");
    printers << QString("Lenovo Pro M");
    printers << QString("Lenovo Pro W");

    ui->label_6->setText(tr("ResStr_Ready"));
    ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                "border:0px solid;"
                                "border-radius:5px;"
                                "background-color: rgb(53, 177, 20);}");


    ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Green.png);");
    errorStatus(false);
    disconnect(ui->deviceNameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_deviceNameBox_currentIndexChanged(int)));
    ui->deviceNameBox->clear();
    connect(ui->deviceNameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_deviceNameBox_currentIndexChanged(int)));
    ui->deviceNameBox->setEnabled(true);
    ui->deviceNameBox->addItem(QString("Lenovo Pro"));
    ui->deviceNameBox->addItem(QString("Lenovo Pro D"));
    ui->deviceNameBox->addItem(QString("Lenovo Pro L"));
    ui->deviceNameBox->addItem(QString("Lenovo Pro M"));
    ui->deviceNameBox->addItem(QString("Lenovo Pro W"));
    qDebug()<<"printers1"<<printers;
#endif


    cycle = new BusyRefreshLabel(ui->cycleWidget,true);
    cycle->setGeometry(385,280,80,80);

    createSysTray();

    QSettings settings;
    QVariant vCRM = settings.value("enableCRM");
    if(vCRM.isNull())
    {
        bool bCRM = false;
        ExperiencePro *exp = new ExperiencePro(this,bCRM);
        exp->exec();
        bCRM = exp->isStartCRM();
        if(bCRM)
        {
            ui->memberCenterWidget->startCRM();
        }
        settings.setValue("enableCRM",bCRM);
    }
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
    quitAction = new QAction(QString("%1(&Q)").arg(tr("ResStr_Exit")),this);
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
//    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
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
    if(enabledWiFi)
    {
        ui->tabStackedWidget->on_btn_WiFi_clicked();
    }
    else
    {
        ui->tabStackedWidget->on_btn_PowerSave_clicked();
    }
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
    if(enabledWiFi)
    {
        ui->tabStackedWidget->on_btn_WiFi_clicked();
    }
    else
    {
        ui->tabStackedWidget->on_btn_PowerSave_clicked();
    }
}

void MainWindow::on_refreshBtn_clicked()
{
    if(ui->tabStackedWidget->currentIndex() != 0 && enabledScanCopy)
    {
        on_Copy_clicked();
    }
    ui->refreshBtn->hide();
//    ui->statusCycle->show();
    ui->statusCycle->startAnimation(20);
    gUInterface->setCmd(UIConfig::CMD_GetPrinters,QString());
//    statusCycle->startAnimation(20);
}

void MainWindow::cmdResult(int cmd,int result ,QVariant data)
{
    switch(cmd){
    case UIConfig::LS_CMD_COPY:
    {
        qDebug()<<"LS_CMD_COPY";
        if(!result)
        {
            isStartCopy = true;
        }
    }
        break;

    case UIConfig::CMD_GetPrinters:{
        if(!result){
            updatePrinter(data);
        }
//        statusCycle->stopAnimation();
        ui->statusCycle->stopAnimation();
        ui->statusCycle->hide();
        ui->refreshBtn->show();
    }
        break;
    case UIConfig::CMD_GetStatus:{
        if(!result){
            LOGLOG("MainWindow")
            updateStatus(data);
        }else{//get status fail
            LOGLOG("get printer status fail!");
            updateStatusPanel(UIConfig::Status_Offline);
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

    int index_of_defaultprinter = 0;
    for(int i = 0 ;i < printerInfos.length() ;i++){
        printerInfo = printerInfos.at(i);
        printers << printerInfo.printer.name;

        ui->deviceNameBox->addItem(printerInfo.printer.name);
        if(printerInfo.printer.isDefault){
            index_of_defaultprinter =  i;
        }
    }
    if(printers.isEmpty()){
        LOGLOG("no printers");
        setcurrentPrinter(QString());
//        errorStatus(true);
        gUInterface->setTimer(0);
        return;
    }else if(printers.contains(current_printer)){
        ui->deviceNameBox->setCurrentIndex(printers.indexOf(current_printer));
    }else{
        ui->deviceNameBox->setCurrentIndex(index_of_defaultprinter);
        setcurrentPrinter(printers.at(index_of_defaultprinter));
    }

    gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);
    gUInterface->setTimer(6);

    if(enabledScanCopy && (!isOfflineStart))
    {
        on_Copy_clicked();
    }
    connect(ui->deviceNameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_deviceNameBox_currentIndexChanged(int)));


}

void MainWindow::setcurrentPrinter(const QString& str)
{
    current_printer = str;
    qDebug()<<"current printer:"<<current_printer;
    gUInterface->setcurrentPrinter(str);
    gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);

    if(str != "")
    {
        PrinterInfo_struct printerInfo = printerInfos.at(ui->deviceNameBox->currentIndex());
        int modelType = UIConfig::getModelSerial(&printerInfo.printer);
        qDebug()<<"printer"<<&printerInfo.printer<<"    modelType:"<<modelType;
        if((modelType & UIConfig::ModelSerial_M) == UIConfig::ModelSerial_M)//M:3in1
        {
            enableMPrinter(true);
            if((modelType & UIConfig::Model_D) == UIConfig::Model_D)//MD:3in1,duplex copy
            {
                ui->tabStackedWidget->setEnabledDuplexCopy(true);
            }
            else
            {
                ui->tabStackedWidget->setEnabledDuplexCopy(false);
            }
        }
        else//L:only print
        {
            enableMPrinter(false);
        }
        if((modelType & UIConfig::Model_W) == UIConfig::Model_W)//W:WIFI
        {
            enabledWiFi = true;
            ui->tabStackedWidget->setEnabledWifi(true);
            if(!isOfflineStart && (!enabledScanCopy))
            {
                ui->tabStackedWidget->on_btn_WiFi_clicked();
            }
        }
        else
        {
            enabledWiFi = false;
            ui->tabStackedWidget->setEnabledWifi(false);
            if(!isOfflineStart && (!enabledScanCopy))
            {
                ui->tabStackedWidget->on_btn_PowerSave_clicked();
            }
        }
    }
}

void MainWindow::enableMPrinter(bool enabled)
{
    if(enabled)
    {
        enabledScanCopy = true;
        ui->tabStackedWidget->setEnabledCopyScan(true);
        ui->Copy->show();
        ui->CopyImgBtn->show();
        ui->Scan->show();
        ui->ScanImgBtn->show();
        ui->Copy->setEnabled(true);
        ui->CopyImgBtn->setEnabled(true);
        ui->Scan->setEnabled(true);
        ui->ScanImgBtn->setEnabled(true);
        ui->Setting->setEnabled(true);
        ui->SettingImgBtn->setEnabled(true);

        QRect sRect = QRect(ui->Scan->geometry().x()+ui->Scan->geometry().width(),ui->Scan->geometry().y(),111,25);
        QRect sIRect = QRect((ui->ScanImgBtn->geometry().x()+ui->ScanImgBtn->geometry().width() - 1),ui->ScanImgBtn->geometry().y(),111,77);
        ui->Setting->setGeometry(sRect);
        ui->SettingImgBtn->setGeometry(sIRect);
    }
    else
    {
        enabledScanCopy = false;
        ui->Copy->hide();
        ui->CopyImgBtn->hide();
        ui->Scan->hide();
        ui->ScanImgBtn->hide();

        ui->Setting->setGeometry(ui->Copy->geometry().x(),ui->Copy->geometry().y(),ui->Copy->geometry().width(),ui->Copy->geometry().height());
        ui->SettingImgBtn->setGeometry(ui->CopyImgBtn->geometry());
        ui->Setting->setStyleSheet(selectState + "border-radius:7px;");
        ui->Setting->setEnabled(false);
        ui->SettingImgBtn->setEnabled(false);

        if(!isOfflineStart)
        {
            ui->tabStackedWidget->setEnabledCopyScan(false);
            ui->tabStackedWidget->setCurrentIndex(2);
        }
    }

    if(isOfflineStart)
    {
        enableTroubleshootingPage(true);
    }
}

void MainWindow::updateStatus(QVariant data)
{
    LOGLOG("updateStatus");
    PrinterInfo_struct printerInfo = data.value<PrinterInfo_struct>();
    PrinterStatus_struct& status = printerInfo.status;
    LOGLOG("get status success:0x%02x" ,status.PrinterStatus);
    onStatusCh(status);
}

void MainWindow::updateOtherStatus(const QString&  ,const PrinterStatus_struct& )
{

}

void MainWindow::on_deviceNameBox_currentIndexChanged(int index)
{
#ifndef DEBUG
    if(printers.at(index) != current_printer)
    {
        updateStatusPanel(UIConfig::Status_Offline);
        current_printer = printers.at(index);
        setcurrentPrinter(printers.at(index));
#endif
#ifdef DEBUG
        qDebug()<<"printers"<<printers;
        if(printers.at(index).endsWith("L"))
        {
            enabledScanCopy = false;
            ui->tabStackedWidget->setEnabledCopyScan(false);
            ui->Copy->hide();
            ui->CopyImgBtn->hide();
            ui->Scan->hide();
            ui->ScanImgBtn->hide();
            ui->tabStackedWidget->setCurrentIndex(2);

            ui->Setting->setGeometry(ui->Copy->geometry());
            ui->SettingImgBtn->setGeometry(ui->CopyImgBtn->geometry());
            ui->Setting->setStyleSheet(selectState + "border-radius:7px;");

            ui->tabStackedWidget->setCurrentIndex(2);
        }
        else
        {
            enabledScanCopy = true;
            ui->tabStackedWidget->setEnabledCopyScan(true);
            ui->Copy->show();
            ui->CopyImgBtn->show();
            ui->Scan->show();
            ui->ScanImgBtn->show();

            QRect sRect = QRect(ui->Scan->geometry().x()+ui->Scan->geometry().width(),ui->Scan->geometry().y(),111,25);
            QRect sIRect = QRect((ui->ScanImgBtn->geometry().x()+ui->ScanImgBtn->geometry().width() - 1),ui->ScanImgBtn->geometry().y(),111,77);
            ui->Setting->setGeometry(sRect);
            ui->SettingImgBtn->setGeometry(sIRect);


            on_Copy_clicked();
        }
        if(printers.at(index).endsWith("W")){
            enabledWiFi = true;
            ui->tabStackedWidget->setEnabledWifi(true);
        }
        else
        {
            enabledWiFi = false;
            ui->tabStackedWidget->setEnabledWifi(false);
        }
        if(printers.at(index).endsWith("D"))
        {
            ui->tabStackedWidget->setEnabledDuplexCopy(true);
        }
        else
        {
            ui->tabStackedWidget->setEnabledDuplexCopy(false);
        }
#endif

        if(ui->tabStackedWidget->currentIndex() != 0)
        {
            LOGLOG("on_deviceNameBox_currentIndexChanged");
            if(enabledScanCopy&&(!isOfflineStart))
            {
                on_Copy_clicked();
            }
        }
#ifndef DEBUG
    }
#endif

}

void MainWindow::enableTroubleshootingPage(bool enabled)
{
    if(enabledScanCopy)
    {
        ui->tabStackedWidget->setCopyStackedWidgetCurrentIndex(enabled);
        ui->CopyImgBtn->setEnabled(!enabled);
        ui->ScanImgBtn->setEnabled(!enabled);
        ui->SettingImgBtn->setEnabled(!enabled);

        ui->Copy->setEnabled(!enabled);
        ui->Scan->setEnabled(!enabled);
        ui->Setting->setEnabled(!enabled);

        if(enabled)
        {
            ui->Copy->setStyleSheet("background-color: white;color:gray;border-top-right-radius:0px;border-bottom-right-radius:0px");
            ui->Scan->setStyleSheet("background-color: white;color:gray;border-radius:0px");
            ui->Setting->setStyleSheet("background-color: white;color:gray;border-top-left-radius:0px;border-bottom-left-radius:0px");
        }
        else
        {

            ui->Copy->setStyleSheet(selectState + "border-top-right-radius:0px;border-bottom-right-radius:0px");
            ui->Scan->setStyleSheet(unSelectState + "border-radius:0px");
            ui->Setting->setStyleSheet(unSelectState + "border-top-left-radius:0px;border-bottom-left-radius:0px");
        }
    }
    else
    {
        if(enabled)
        {
            ui->tabStackedWidget->setCopyStackedWidgetCurrentIndex(1);
            ui->Setting->setStyleSheet("background-color: white;color:gray;border-radius:7px;");
        }
        else
        {
            ui->tabStackedWidget->setCopyStackedWidgetCurrentIndex(0);
            enableAllFunction(false);
            ui->tabStackedWidget->setEnabledCopyScan(false);
            ui->tabStackedWidget->setCurrentIndex(2);
            ui->Setting->setStyleSheet(selectState + "border-radius:7px;");
        }
    }
}

void MainWindow::updateTonerCarStatus(int toner)
{
    if(toner<0)
    {
        ui->btCar->setStyleSheet("border-image: url(:/Images/shopCart_Disable.tif);");
        ui->btCar->setEnabled(false);
    }
    else if(toner < 11)
    {
        ui->btCar->setStyleSheet("border-image: url(:/Images/shopCart_Warn.png);");
        ui->btCar->setEnabled(true);
    }
    else if(toner < 30)
    {
        ui->btCar->setStyleSheet("border-image: url(:/Images/shopCart_Normal.png);");
        ui->btCar->setEnabled(true);
    }
    else
    {
        ui->btCar->setStyleSheet("border-image: url(:/Images/shopCart_Normal.png);");
        ui->btCar->setEnabled(true);
    }
}

void MainWindow::enableAllFunction(bool enabled)
{
    if(enabledScanCopy)
    {
        ui->tabStackedWidget->set_setting_enabled(enabled);
        ui->tabStackedWidget->set_scan_enabled(enabled);//Added for default enable scan button by gavin 2016-04-14
        ui->tabStackedWidget->set_copy_enabled(enabled);
    }
    else
    {
        ui->tabStackedWidget->set_setting_enabled(enabled);
    }
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

void MainWindow::onStatusCh(const PrinterStatus_struct& status)
{
    if(status.PrinterStatus == UIConfig::Usb_Locked){
        return;
    }else if(status.PrinterStatus == UIConfig::Usb_Printing){
        QString errMsg = UIConfig::getErrorMsg(UIConfig::Printing ,UIConfig::UnknowJob,0);
        ui->label_10->setText(errMsg);
        return;
    }else if(status.PrinterStatus == UIConfig::Usb_Scanning){
        QString errMsg = UIConfig::getErrorMsg(UIConfig::ScanScanning ,UIConfig::UnknowJob,0);
        ui->label_10->setText(errMsg);
        return;
    }
    ui->label_10->setStyleSheet("QLabel{color:break;}");
    ui->mofenProgressBar->setValue(status.TonelStatusLevelK);
    updateTonerCarStatus(status.TonelStatusLevelK);

    int displayStatus = UIConfig::GetStatusTypeForUI((UIConfig::EnumStatus)status.PrinterStatus);
    QString errMsg = UIConfig::getErrorMsg((UIConfig::EnumStatus)status.PrinterStatus,UIConfig::UnknowJob,0);

    ui->label_10->setText(errMsg);
    set_Message_Background_Color((UIConfig::EnumStatus)status.PrinterStatus);
    updateStatusPanel(displayStatus);
}

void MainWindow::updateStatusPanel(int status)
{
    switch (status) {
    case UIConfig::Status_Ready:
        qDebug()<<"Status_Ready";
        ui->label_6->setText(tr("ResStr_Ready"));
        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(53, 177, 20);}");
        enableAllFunction(true);
        ui->errorBtn->hide();
        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Green.png);");

        if(isOfflineStart)
        {
            isOfflineStart = false;
            enableTroubleshootingPage(false);
        }
        emit signalCloseAnimationDlg();
        if(isStartCopy)
        {
            isStartCopy = false;
            ui->tabStackedWidget->recoverCopyMode();
        }
        break;
    case UIConfig::Status_Sleep:
        ui->label_6->setText(tr("ResStr_Sleep"));
        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(53, 177, 20);}");
        enableAllFunction(true);
        ui->errorBtn->hide();
        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Green.png);");
        if(isOfflineStart)
        {
            isOfflineStart = false;
            enableTroubleshootingPage(false);
        }
        emit signalCloseAnimationDlg();
        break;
    case UIConfig::Status_Offline:
        ui->label_6->setText(tr("ResStr_Offline"));
        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(110, 110, 110);}");
        enableAllFunction(false);
        ui->errorBtn->hide();
        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Gray.png);");
        ui->mofenProgressBar->setValue(0);
        updateTonerCarStatus(-1);
        emit signalCloseAnimationDlg();
        break;
    case UIConfig::Status_Busy:
        ui->label_6->setText(tr("ResStr_Busy"));
        ui->label_6->setStyleSheet("QLabel{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: rgb(53, 177, 20);}");
        ui->errorBtn->hide();
        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Green.png);");
        if(isOfflineStart)
        {
            isOfflineStart = false;
            enableTroubleshootingPage(false);
        }
        emit signalCloseAnimationDlg();
        break;
    case UIConfig::Status_Error:
        ui->label_6->setText(tr("ResStr_Error"));
        ui->label_6->setStyleSheet("QLabel{color: white;"
                                    "border:0px solid;"
                                    "border-radius:5px;"
                                    "background-color: red;}");
        enableAllFunction(true);
        ui->errorBtn->show();
        ui->pushButton->setStyleSheet("border-image: url(:/Images/LED_Red.png);");
        if(isOfflineStart)
        {
            isOfflineStart = false;
            enableTroubleshootingPage(false);
        }
        break;
    default:
        break;
    }
}

void MainWindow::setDeviceMsg(const QString& msg, int result)
{
    if(!result)
        ui->label_10->setStyleSheet("QLabel{color:black}");
    else
        ui->label_10->setStyleSheet("QLabel{color:red}");

    ui->label_10->setText(msg);
}

void MainWindow::enableCycleAnimation(bool enabled)
{
    if(enabled)
    {
        qDebug()<<"startCycleAnimation";
        ui->cycleWidget->show();
        cycle->startAnimation(20);
    }
    else
    {
        qDebug()<<"stopCycleAnimation";
        ui->cycleWidget->hide();
        cycle->stopAnimation();
    }
}

//void MainWindow::startCycleAnimation()
//{
//    qDebug()<<"startCycleAnimation";
//    ui->cycleWidget->show();
//    cycle->startAnimation(20);
//}

//void MainWindow::stopCycleAnimation()
//{
//    qDebug()<<"stopCycleAnimation";
//    ui->cycleWidget->hide();
//    cycle->stopAnimation();
//}

void MainWindow::startScan()
{
    ui->CopyImgBtn->setEnabled(false);
    ui->ScanImgBtn->setEnabled(false);
    ui->SettingImgBtn->setEnabled(false);
    ui->Copy->setEnabled(false);
    ui->Scan->setEnabled(false);
    ui->Setting->setEnabled(false);

    ui->refreshBtn->setEnabled(false);

    ui->deviceNameBox->setEnabled(false);
}

void MainWindow::stopScan()
{
    ui->CopyImgBtn->setEnabled(true);
    ui->ScanImgBtn->setEnabled(true);
    ui->SettingImgBtn->setEnabled(true);
    ui->Copy->setEnabled(true);
    ui->Scan->setEnabled(true);
    ui->Setting->setEnabled(true);

    ui->refreshBtn->setEnabled(true);

    ui->deviceNameBox->setEnabled(true);
}

void MainWindow::on_errorBtn_clicked()
{
    bool enNextShow = false;
    AnimationDlg *aDialog = new AnimationDlg(this, 0xBD, &enNextShow);
    connect(this,SIGNAL(signalCloseAnimationDlg()),aDialog,SLOT(close()));
    aDialog->setAttribute(Qt::WA_DeleteOnClose);
    if (aDialog->exec() == QDialog::Rejected)
    {
        return;
    }
}

void MainWindow::on_btCar_clicked()
{
    if(ui->memberCenterWidget->loginPhone !="")
    {
        QString url = QString("http://ibase.lenovoimage.com/buy_abc2.aspx?id=%0").arg(ui->memberCenterWidget->loginPhone);
        QDesktopServices::openUrl(QUrl(url));
    }
    else
    {
        QDesktopServices::openUrl(QUrl("http://ibase.lenovoimage.com/buy_abc2.aspx"));
    }

}
