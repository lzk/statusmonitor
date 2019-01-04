#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "uinterface.h"
#include "membercenter/about.h"
#include "promptdialog.h"
#include "ids_string.h"
#include "animationdlg.h"
#include <qmenu.h>


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

    gUInterface->setCmd(UIConfig::CMD_GetPrinters,QString());

    statusCycle = new BusyRefreshLabel(ui->deviceMsgWidget,true);
    statusCycle->setGeometry(50,30,20,19);

    ui->cycleWidget->hide();
    ui->errorBtn->hide();
//    ui->tabStackedWidget->setCopyStackedWidgetCurrentIndex(1);
//    ui->CopyImgBtn->setEnabled(false);
//    ui->ScanImgBtn->setEnabled(false);
//    ui->SettingImgBtn->setEnabled(false);
//    ui->Copy->setStyleSheet("background-color: white;color:gray;border-top-right-radius:0px;border-bottom-right-radius:0px");
//    ui->Scan->setStyleSheet("background-color: white;color:gray;border-radius:0px");
//    ui->Setting->setStyleSheet("background-color: white;color:gray;border-top-left-radius:0px;border-bottom-left-radius:0px");
//    ui->Copy->setEnabled(false);
//    ui->Scan->setEnabled(false);
//    ui->Setting->setEnabled(false);

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
        PrinterInfo_struct printerInfo = data.value<PrinterInfo_struct>();
        PrinterStatus_struct& status = printerInfo.status;
        if(!result){
            LOGLOG("get status success:0x%02x" ,status.PrinterStatus);
        }else{//get status fail
            LOGLOG("get printer status fail!");
            memset(&status ,-1 ,sizeof(status));
//                status.PrinterStatus = -1;
        }
        on_status_ch(status);
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
    }else if(printers.contains(current_printer)){
        ui->deviceNameBox->setCurrentIndex(printers.indexOf(current_printer));
    }else{
        setcurrentPrinter(printers.first());
    }

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


void MainWindow::updateStatus(const PrinterStatus_struct& status)
{

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
            on_Copy_clicked();
        }
    }

}

void MainWindow::on_status_ch(const PrinterStatus_struct& status)
{

    ui->tabStackedWidget->set_setting_enabled(true);

    if(ui->deviceNameBox->isEnabled())
    {
        ui->tabStackedWidget->set_scan_enabled(true);//Added for default enable scan button by gavin 2016-04-14
        ui->tabStackedWidget->set_copy_enabled(true);
    }
    ui->label_10->setStyleSheet("QLabel{color:break;}");
    ui->mofenProgressBar->setValue(status.TonelStatusLevelK);
    ui->errorBtn->hide();

//    switch (status.PrinterStatus) {
//    case PS_READY:
//        ui->label_6->setText(tr("ResStr_Ready"));
//        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
//                                    "border:0px solid;"
//                                    "border-radius:5px;"
//                                    "background-color: rgb(53, 177, 20);}");
//        //ui->label_10->setText(devStatus->getDevMsg());
//        break;
//    case PS_POWER_SAVING:
//        ui->label_6->setText(tr("ResStr_Sleep"));
//        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
//                                    "border:0px solid;"
//                                    "border-radius:5px;"
//                                    "background-color: rgb(53, 177, 20);}");
//       // ui->label_10->setText(devStatus->getDevMsg());
//        break;
//    case PS_OFFLINE:
//        ui->label_6->setText(tr("ResStr_Offline"));
//        ui->label_6->setStyleSheet("QLabel#label_6{color: white;"
//                                    "border:0px solid;"
//                                    "border-radius:5px;"
//                                    "background-color: rgb(110, 110, 110);}");
//        //ui->label_10->setText(devStatus->getDevMsg());
//        qDebug()<<"set_copy_enabled false";
//        ui->tabStackedWidget->set_copy_enabled(false);
//        ui->tabStackedWidget->set_setting_enabled(false);
//        ui->tabStackedWidget->set_scan_enabled(false); //Added for disable scan button when offline by gavin 2016-04-14
//        break;
////    case PS_READY:
////        ui->label_6->setText(tr("ResStr_Ready")); //device status is warning, the ui status is ready in spec
////        ui->label_6->setStyleSheet("QLabel{color: white;"
////                                    "border:0px solid;"
////                                    "border-radius:5px;"
////                                    "background-color: rgb(53, 177, 20);}");
//////        ui->label_10->setText(devStatus->getDevMsg());
////        break;
//    case PS_BUSY:
//        ui->label_6->setText(tr("ResStr_Busy"));
//        ui->label_6->setStyleSheet("QLabel{color: white;"
//                                    "border:0px solid;"
//                                    "border-radius:5px;"
//                                    "background-color: rgb(53, 177, 20);}");
//        //ui->label_10->setText(devStatus->getDevMsg());

//        break;
//    case PS_ERROR_ERROR:
//        ui->label_6->setText(tr("ResStr_Error"));
//        ui->label_6->setStyleSheet("QLabel{color: white;"
//                                    "border:0px solid;"
//                                    "border-radius:5px;"
//                                    "background-color: red;}");
//        //ui->label_10->setText(devStatus->getDevMsg());
//        ui->label_10->setStyleSheet("QLabel{color:red;}");
//        ui->errorBtn->show();
//        break;
//    default:
//        break;
//    }
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
