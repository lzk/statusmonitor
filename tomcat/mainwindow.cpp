#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include "uinterface.h"
#include <QDateTime>
#include <unistd.h>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>
#include "commonapi.h"
#include "statuspaser.h"
#define TEST 0
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,about_dialog(NULL)
  ,bFindOnLinePrinter(false)
  ,bCheckPrinter(true)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(3);
    setWindowTitle(app_name);
//    setWindowIcon(QIcon(":/image/app_icon.png"));
//    ui->menuBar->hide();
//    ui->statusBar->hide();
    ui->mainToolBar->hide();

    this->setFixedSize(520 ,450);
    ui->statusUpdate_groupBox->hide();
    ui->tableWidget_jobs->setFocusPolicy(Qt::ClickFocus);
//    ui->tableWidget_printers->setFocusPolicy(Qt::NoFocus);

    createSysTray();
    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

    QVariant value;
    appSettings("record" ,value ,QVariant(false));
    record_printlist = value.toBool();
    ui->checkBox_record->setChecked(record_printlist);

    //init
    PrinterStatus_struct status;
    memset(&status ,-1 ,sizeof(status));
    updateToner(status.TonelStatusLevelC ,status.TonelStatusLevelM ,status.TonelStatusLevelY ,status.TonelStatusLevelK);
    updateStatus(status);

//    gUInterface->setCmd(UIConfig::CMD_GetPrinters ,QString());


}

MainWindow::~MainWindow()
{
    delete ui;
    if(about_dialog){
        delete about_dialog;
    }
}

void MainWindow::about()
{
    if(!about_dialog){
        about_dialog = new About;
    }
//    if(about_dialog->isHidden())
        about_dialog->show();
        about_dialog->raise();
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
    aboutAction = new QAction(tr("关于..."), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    quitAction = new QAction(tr("退出(&Q)"), this);
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    trayIconMenu = new QMenu(this);
//    trayIconMenu->addAction(minimizeAction);
//    trayIconMenu->addAction(maximizeAction);
//    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(aboutAction);
//    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/image/TrayReady.png"));

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:{
        if(printers.isEmpty()){

            QMessageBox message_box;
            message_box.setButtonText(QMessageBox::Ok ,"确定");
            message_box.setIcon(QMessageBox::Critical);
            message_box.setText("系统中未安装打印机。");
            message_box.setWindowTitle(" ");
            message_box.exec();
        }else{
            showNormal();
        }
    }
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
        if(!printerInfo.printer.name[0] || !current_printer.compare(printerInfo.printer.name)){
#if TEST
    status.PrinterStatus = 0;
    status.TonelStatusLevelC = 80;
    status.TonelStatusLevelM = 100;
    status.TonelStatusLevelY = 10;
    status.TonelStatusLevelK = 30;
#endif
            updateToner(status.TonelStatusLevelC ,status.TonelStatusLevelM ,status.TonelStatusLevelY ,status.TonelStatusLevelK);
            updateStatus(status);
        }
//        updateOtherStatus(printerInfo.printer.name ,status);
    }
        break;

    case UIConfig::CMD_GetPrinters:{
        if(!result){
            updatePrinter(data);
        }
    }
        break;

    case UIConfig::CMD_GetJobs:{
        if(!result){
            updateJobHistory(data);
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::setcurrentPrinter(const QString& str)
{
    current_printer = str;
    gUInterface->setcurrentPrinter(str);
    setWindowTitle(app_name + " - " + str);
//    gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);
}

void MainWindow::on_checkBox_clicked()
{

    if(ui->checkBox->isChecked()){
        gUInterface->setTimer(ui->lineEdit_updateInterval->text().toInt());
    }else{
        gUInterface->setTimer();
    }
}

int getEnterPassword(QString& password);
int MainWindow::validate_password()
{
    int ret;
    QString password;
    while (true) {
        ret = getEnterPassword(password);
        if(!ret && !password.isEmpty()){
            QVariant sys_password;
            appSettings("password" ,sys_password ,QVariant(QString("1234ABCD")));
            if(!password.compare(sys_password.toString())){
                break;
            }else{
                QMessageBox message_box;
                message_box.setButtonText(QMessageBox::Ok ,"确定");
                message_box.setIcon(QMessageBox::Critical);
                message_box.setText("密码错误");
                message_box.setWindowTitle(" ");
                message_box.exec();
            }
        }else{
            ret = -1;
            break;
        }
    }
    return ret;
}
void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
    case 1:
        gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);
        break;
    case 2:
#if !TEST
    {
        ui->btn_nextpage->setEnabled(false);
        ui->btn_prepage->setEnabled(false);

//        ui->tableWidget_jobs->clearContents();
        ui->tableWidget_jobs->setRowCount(0);
        if(!validate_password()){
            jobs_page = 0;
            gUInterface->setCmd(UIConfig::CMD_GetJobs ,current_printer ,jobs_page);
        }
    }
#else
    {
        ui->tableWidget_jobs->setRowCount(1);
        QTableWidgetItem* item;
        item = new QTableWidgetItem(tr("%1").arg(QString("TOEC_Printer")));
        ui->tableWidget_jobs->setItem(0 ,0 ,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("TOEC")));
        ui->tableWidget_jobs->setItem(0 ,1,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("TOEC User")));
        ui->tableWidget_jobs->setItem(0 ,2,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("toec.txt")));
        ui->tableWidget_jobs->setItem(0 ,3,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("1")));
        ui->tableWidget_jobs->setItem(0 ,4,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("1")));
        ui->tableWidget_jobs->setItem(0 ,5,item);
        item = new QTableWidgetItem(tr("%1").arg(QDateTime::currentDateTime().toString()));
        ui->tableWidget_jobs->setItem(0 ,6,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("是")));
        ui->tableWidget_jobs->setItem(0 ,7,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("成功")));
        ui->tableWidget_jobs->setItem(0 ,8,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("否")));
        ui->tableWidget_jobs->setItem(0 ,9,item);

    }
#endif
        break;
    case 3:
#if !TEST
        gUInterface->setCmd(UIConfig::CMD_GetPrinters ,QString());
#else
    {
        ui->tableWidget_printers->setRowCount(1);
        QTableWidgetItem* item;
        item = new QTableWidgetItem(tr("%1").arg(QString("TOEC_Printer")));
        ui->tableWidget_printers->setItem(0 ,0 ,item);
//        item = new QTableWidgetItem(tr("%1").arg(QString("节电中")));
//        ui->tableWidget_printers->setItem(0 ,1,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("OEP3300CDN")));
        ui->tableWidget_printers->setItem(0 ,2,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("192.168.2.20")));
        ui->tableWidget_printers->setItem(0 ,3,item);

    }
#endif
        break;
    default:
        break;
    }
}

void MainWindow::on_tableWidget_printers_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    setcurrentPrinter(ui->tableWidget_printers->item(row ,0)->text());
}

void MainWindow::updateToner(int c ,int m ,int y ,int k)
{
    QString c_uri = ":/image/";
    QString m_uri = ":/image/";
    QString y_uri = ":/image/";
    QString k_uri = ":/image/";
    if(c < 0){
        c_uri += "t_unknown";
    }else if(c == 0){
        c_uri += "t_0";
    }else if(c < 10){
        c_uri += "c5";
    }else if(c < 20){
        c_uri += "c10";
    }else if(c < 30){
        c_uri += "c20";
    }else if(c < 40){
        c_uri += "c30";
    }else if(c < 50){
        c_uri += "c40";
    }else if(c < 60){
        c_uri += "c50";
    }else if(c < 70){
        c_uri += "c60";
    }else if(c < 80){
        c_uri += "c70";
    }else if(c < 90){
        c_uri += "c80";
    }else if(c < 100){
        c_uri += "c90";
    }else{
        c_uri += "c100";
    }
    if(m < 0){
        m_uri += "t_unknown";
    }else if(m == 0 ){
        m_uri += "t_0";
    }else if(m < 10){
        m_uri += "m5";
    }else if(m < 20){
        m_uri += "m10";
    }else if(m < 30){
        m_uri += "m20";
    }else if(m < 40){
        m_uri += "m30";
    }else if(m < 50){
        m_uri += "m40";
    }else if(m < 60){
        m_uri += "m50";
    }else if(m < 70){
        m_uri += "m60";
    }else if(m < 80){
        m_uri += "m70";
    }else if(m < 90){
        m_uri += "m80";
    }else if(m < 100){
        m_uri += "m90";
    }else{
        m_uri += "m100";
    }
    if(y < 0){
        y_uri += "t_unknown";
    }else if(y == 0){
        y_uri += "t_0";
    }else if(y < 10){
        y_uri += "y5";
    }else if(y < 20){
        y_uri += "y10";
    }else if(y < 30){
        y_uri += "y20";
    }else if(y < 40){
        y_uri += "y30";
    }else if(y < 50){
        y_uri += "y40";
    }else if(y < 60){
        y_uri += "y50";
    }else if(y < 70){
        y_uri += "y60";
    }else if(y < 80){
        y_uri += "y70";
    }else if(y < 90){
        y_uri += "y80";
    }else if(y < 100){
        y_uri += "y90";
    }else{
        y_uri += "y100";
    }
    if(k < 0){
        k_uri += "t_unknown";
    }else if(k == 0){
        k_uri += "t_0";
    }else if(k < 10){
        k_uri += "k5";
    }else if(k < 20){
        k_uri += "k10";
    }else if(k < 30){
        k_uri += "k20";
    }else if(k < 40){
        k_uri += "k30";
    }else if(k < 50){
        k_uri += "k40";
    }else if(k < 60){
        k_uri += "k50";
    }else if(k < 70){
        k_uri += "k60";
    }else if(k < 80){
        k_uri += "k70";
    }else if(k < 90){
        k_uri += "k80";
    }else if(k < 100){
        k_uri += "k90";
    }else{
        k_uri += "k100";
    }
    c_uri += ".png";
    m_uri += ".png";
    y_uri += ".png";
    k_uri += ".png";
    ui->label_tonerC->setPixmap(QPixmap(c_uri));
    ui->label_tonerM->setPixmap(QPixmap(m_uri));
    ui->label_tonerY->setPixmap(QPixmap(y_uri));
    ui->label_tonerK->setPixmap(QPixmap(k_uri));
}

QString MainWindow::get_Status_string(const PrinterStatus_struct& status)
{
    QString str_status;
    ErrorInfo_struct ei = getErrorInfo(status.ErrorCodeGroup, status.ErrorCodeID, status.PaperType, status.PaperSize, status.PaperTray);
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
            if(current_printer.isEmpty())
                str_status = IDS_STRUnknow;
            else
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
#include <QMovie>
void MainWindow::updateStatus(const PrinterStatus_struct& status)
{
    QString text;
    int i;
    QString toner_text;
    const PrinterStatus_struct *ps = &status;

    int currStatus = status.PrinterStatus;
    //update toner text
/*    bool bShowLowTonerAlert = !!status.LowTonerAlert; // BMS#51330
    if (bShowLowTonerAlert && !IsStatusUnknownToner(currStatus) && StatusPaser::AnyTonerReachLevel1(status) && !StatusPaser::IsNonDellTonerMode(status)) {
        if (StatusPaser::OnlyColorTonerEmpty(status)) {
            if (IsStatusPrinting(currStatus))
                toner_text = IDS_TONER_NEED_REPLACE;
            else
                toner_text = QString();
        }
        else {
            toner_text = IDS_TONER_IS_GETTING_LOW;
        }
        ui->label_toner->setPixmap(QPixmap(":/image/toner.png"));
        ui->label_toner->show();
    }
    else {
        toner_text = QString();
        ui->label_toner->hide();
    }
    ui->textEdit_toner->setText(toner_text);
*/
    if(status.TonelStatusLevelC <20 || status.TonelStatusLevelK <20 || status.TonelStatusLevelM <20 || status.TonelStatusLevelY <20)
    {
        toner_text = "请更换碳粉盒。";
        ui->label_toner->setPixmap(QPixmap(":/image/toner.png"));
        ui->textEdit_toner->setText(toner_text);
        ui->textEdit_toner->show();
        ui->label_toner->show();
    }
    else
    {
        ui->label_toner->hide();
        ui->textEdit_toner->hide();
    }



    //update toner picture
    QString toner_pic = ":/image/";
    if(currStatus == PS_ERROR_DOOR_OPEN){
        toner_pic += "OpenCover.gif";
        QMovie* movie = new QMovie(toner_pic);
        ui->label_printer->setMovie(movie);
        movie->start();
    }else{

        switch (currStatus) {
        case PS_ERROR_TRAY_DETACHED:
            toner_pic += "NoCassete";
            break;
        case PS_ERROR_DOOR_OPEN:
            break;
        case PS_ERROR_OUT_OF_PAPER:
            toner_pic += "Load_Paper";
            break;
        case PS_ERROR_PAPER_PROBLEM:
            toner_pic += "Load_Paper";
            break;
        case PS_ERROR_PAPER_JAM:
            if (ps->ErrorCodeGroup == 5 && ps->ErrorCodeID == 15) // Jam at input tray
                toner_pic += "MisfeedJAM";
            else
                toner_pic += "Rear_PaperJam";
            break;
        case PS_ERROR_ERROR:
            toner_pic += "printer";
            break;

            //	case PS_ERROR_ADF_COVER_OPEN:
            //DrawTransparentBmps(hDC, 1, bmpID_adfcoveropen, rect, offset_adfcoveropen, RGB(255,0,255));
            //		DrawBmpList(hDC,adfOpen,rect, RGB(255,0,255));
            //		break;
            //	case PS_ERROR_ADF_PAPER_JAM:
            //DrawTransparentBmps(hDC, 1, bmpID_adfpaperjam, rect, offset_adfpaperjam, RGB(255,0,255));
            //		DrawBmpList(hDC,adfJam,rect, RGB(255,0,255));
            //		break;
        case PS_ERROR_PAGE_ERROR:
        case PS_ERROR_POWER_OFF:
        case PS_OFFLINE:
        case PS_ERROR_OUT_OF_MEMORY:
        case PS_ERROR_NOT_AVAILABLE:
        case PS_ERROR_NOT_SUPPORT:
        case PS_ERROR_OUTPUT_BIN_FULL:
        case PS_UNKNOWN:
            toner_pic += "printer";
            break;
        case PS_POWER_SAVING:
        case PS_WARMING_UP:
        case PS_PRINTING:
        case PS_PENDING_DELETION:
        case PS_WAITING:
        case PS_INITIALIZING:
        case PS_BUSY:
            toner_pic += "printer";
            break;
        case PS_ERROR_NO_TONER:
            toner_pic += "toner";
            break;
        case PS_ERROR_USER_INTERVENTION_REQUIRED:
            if (ps->ErrorCodeGroup == 5 && (ps->ErrorCodeID >= 2 && ps->ErrorCodeID <= 9)) {	// Toner not installed
                toner_pic += "toner";
            }
            else if (ps->ErrorCodeGroup == 5 && (ps->ErrorCodeID >= 57 && ps->ErrorCodeID <= 60)) { // Unsupported Toner detected
                toner_pic += "toner";
            }
            else if (ps->ErrorCodeGroup == 4 && ps->ErrorCodeID == 3) { // 04-003
                toner_pic += "printer";
            }
            else { // Maybe there are some other cases (TBD)
                   //DrawTransparentBmps(hDC, 2, bmpID_error, rect, offset_error, RGB(255,0,255));
                toner_pic += "printer";
            }

            break;
        default:
            toner_pic += "printer";
            break;
        }
        toner_pic += ".png";
        ui->label_printer->setPixmap(QPixmap(toner_pic));
    }

    bool showDetail;
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

    int warning_status = 0;
    QString status_icon = ":/image/";
//    switch (currStatus) {
//	case PS_READY:
//	default:
//        warning_status = 0;
//        break;
//    case PS_PRINTING:
//        warning_status = 0;
//        break;
//	case PS_POWER_SAVING:
//        warning_status = 0;
//        break;
//	case PS_WARMING_UP:
//        warning_status = 0;
//        break;
//	case PS_PENDING_DELETION:
//        warning_status = 0;
//        break;
//	case PS_PAUSED:
//        warning_status = 0;
//        break;
//	case PS_WAITING:
//        warning_status = 0;
//        break;
//	case PS_PROCESSING:
//        warning_status = 0;
//        break;
//	case PS_BUSY:
//        warning_status = 0;
//        break;
//	case PS_OFFLINE:
//        warning_status = 1;
//        break;
//	case PS_TONER_LOW:
//        warning_status = 1;
//        break;
//	case PS_INITIALIZING:
//        warning_status = 1;
//        break;
//	case PS_UNKNOWN:
//        warning_status = 1;
//        break;
//	case PS_ACTIVE:
//        warning_status = 1;
//        break;
//	case PS_MANUAL_FEED_REQUIRED:
//        warning_status = 1;
//        break;
//	case PS_ERROR_PAPER_JAM:
//        warning_status = 2;
//        break;
//	case PS_ERROR_DOOR_OPEN:
//        warning_status = 2;
//        break;
//	case PS_ERROR_ADF_COVER_OPEN:
//        warning_status = 2;
//        break;
//	case PS_ERROR_ADF_PAPER_JAM:
//        warning_status = 2;
//        break;
//	case PS_ERROR_OUT_OF_MEMORY:
//        warning_status = 2;
//        break;
//	case PS_ERROR_OUT_OF_PAPER:
//        warning_status = 2;
//        break;
//	case PS_ERROR_PAPER_PROBLEM:
//        warning_status = 2;
//        break;
//	case PS_ERROR_NO_TONER:
//        warning_status = 2;
//        break;
//	case PS_ERROR_PAGE_ERROR:
//        warning_status = 2;
//        break;
//	case PS_ERROR_NOT_AVAILABLE:
//        warning_status = 2;
//        break;
//	case PS_ERROR_OUTPUT_BIN_FULL:
//        warning_status = 2;
//        break;
//	case PS_ERROR_NOT_SUPPORT:
//        warning_status = 2;
//        break;
//	case PS_ERROR_USER_INTERVENTION_REQUIRED:
//        warning_status = 2;
//        break;
//	case PS_ERROR_POWER_OFF:
//        warning_status = 2;
//        break;
//	case PS_ERROR_TRAY_DETACHED:
//        warning_status = 1;
//        break;
//	case PS_ERROR_ERROR:
//        warning_status = 2;
//        break;
//	}
    if(IsStatusError(currStatus)){
		if((currStatus == PS_ERROR_TRAY_DETACHED &&  status.ErrorCodeGroup == 0x05 && status.ErrorCodeID == 72) ||(currStatus == PS_ERROR_OUT_OF_PAPER &&  status.ErrorCodeGroup == 0x05 && status.ErrorCodeID == 73))
			warning_status = 1;
		else
          warning_status = 2;
    }else if(IsStatusAbnormal(currStatus) || (currStatus == PS_READY &&  status.ErrorCodeGroup == 0x05 && (status.ErrorCodeID == 72 || status.ErrorCodeID == 73 )))
    {
        warning_status = 1;
    }
    switch (warning_status) {
    case 1:
        trayIcon->setIcon(QIcon(":/image/TrayWarnning.png"));
        status_icon += "status_warning";
        break;
    case 2:
        trayIcon->setIcon(QIcon(":/image/TrayError.png"));
        status_icon += "status_error";
        break;
    case 0:
    default:
        trayIcon->setIcon(QIcon(":/image/TrayReady.png"));
        status_icon += "status_normal";
        break;
    }
    status_icon += ".png";
    //update status string
    text = "<html><head/><body>";

    ErrorInfo_struct ei = getErrorInfo(status.ErrorCodeGroup ,status.ErrorCodeID ,status.PaperType ,status.PaperSize,status.PaperTray);
        if(!ei.error || !ei.errorString){
            text += QString() + "<p>" + get_Status_string(status) + "</p>";
    //        text += QString() + "<p><img src=\"" +status_icon + "\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + get_Status_string(status) + "</p>";
            text += "</body></html>";
            ui->label_status->setText(text);
            ui->label_detail->setText("");
        }else{

            if ((IsStatusError(currStatus) && !IsStatusVirtual(currStatus) && currStatus != PS_ERROR_NOT_AVAILABLE && currStatus != PS_ERROR_NOT_SUPPORT) || currStatus == PS_TONER_LOW || currStatus != PS_POWER_SAVING)
            {
                if(currStatus == PS_ERROR_NO_TONER ||currStatus == PS_TONER_LOW)
                {
                    if((status.TonelStatusLevelC <20 && status.TonelStatusLevelK >20 && status.TonelStatusLevelM>20 && status.TonelStatusLevelY >20)
                        ||(status.TonelStatusLevelC >20 && status.TonelStatusLevelK <20 && status.TonelStatusLevelM>20 && status.TonelStatusLevelY >20)
                        ||(status.TonelStatusLevelC >20 && status.TonelStatusLevelK >20 && status.TonelStatusLevelM<20 && status.TonelStatusLevelY >20)
                        ||(status.TonelStatusLevelC >20 && status.TonelStatusLevelK >20 && status.TonelStatusLevelM>20 && status.TonelStatusLevelY <20))
                    {
                        text += QString() + "<p>" + ei.errorString->title + "</p>";
                //        text += QString() + "<p><img src=\"" +status_icon + "\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + ei.errorString->title + "</p>";
                //        text += "<br/>";
                        text += QString() + "<p>" + ei.error->code + "</p>";
                        text += "</body></html>";
                        ui->label_status->setText(text);

                        text = "<html><head/><body>";
                        const char* extra_string = "";
                        for(i = 0 ;i < ei.errorString->lines ;i++){
                //            text += "<br/>";
                            if(ei.errorString->mediaInfo){
                                if(i == ei.errorString->lines -2)
                                    extra_string = ei.paperSizeString;
                                else if(i == ei.errorString->lines - 1)
                                    extra_string = ei.paperTypeString;
                            }
                            switch (i) 
							{
                            case 0:
                                text += QString() + "<p>" + ei.errorString->line0 + extra_string + "</p>";
                                break;
                            case 1:
                                text += QString() + "<p>" + ei.errorString->line1 + extra_string + "</p>";
                                break;
                            case 2:
                                text += QString() + "<p>" + ei.errorString->line2 + extra_string + "</p>";
                                break;
                            case 3:
                                text += QString() + "<p>" + ei.errorString->line3 + extra_string + "</p>";
                                break;
                            case 4:
                                text += QString() + "<p>" + ei.errorString->line4 + extra_string + "</p>";
                                break;
                            default:
                                break;
                            }
                        }
                        text += "</body></html>";
                        ui->label_detail->setText(text);
                    }
                    else
                    {
                        QString TonerColor ="";
                        QString TonerStatus ="";
                        char cTonerColor[256];
                        memset(cTonerColor ,0 ,sizeof(cTonerColor));
                        if(status.TonelStatusLevelK< 20)
                        {
                            if (strlen(cTonerColor)>0)
                                 strcat(cTonerColor, "、");
                            strcat(cTonerColor, IDS_BLACK);
                        }
                        if(status.TonelStatusLevelC< 20)
                        {
                            if (strlen(cTonerColor)>0)
                                 strcat(cTonerColor, "、");
                            strcat(cTonerColor, IDS_CYAN);
                        }
                        if(status.TonelStatusLevelM< 20)
                        {
                            if (strlen(cTonerColor)>0)
                                 strcat(cTonerColor, "、");
                            strcat(cTonerColor, IDS_MAGENTA);
                        }
                        if(status.TonelStatusLevelY< 20)
                        {
                            if (strlen(cTonerColor)>0)
                                 strcat(cTonerColor, "、");
                            strcat(cTonerColor, IDS_YELLOW);
                        }
                        switch (currStatus)
                        {
                        case PS_TONER_LOW:
                             TonerStatus = QString().sprintf("需要立即更换%s粉仓。",cTonerColor);
                            break;
                        case PS_ERROR_NO_TONER:
                             TonerStatus = QString().sprintf("更换%s粉仓。",cTonerColor);
                            break;
                        }
                       QString TonerRecoveryStr = QString().sprintf("打开碳粉盒入口盖。然后卸下所用的%s粉仓并安装新的%s粉仓。" ,cTonerColor ,cTonerColor);
                        ui->label_status->setText(TonerStatus);
                        ui->label_detail->setText(TonerRecoveryStr);
                    }
                }
                else
                {					
                    text += QString() + "<p>" + ei.errorString->title + "</p>";
            //        text += QString() + "<p><img src=\"" +status_icon + "\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + ei.errorString->title + "</p>";
            //        text += "<br/>";
                    text += QString() + "<p>" + ei.error->code + "</p>";
                    text += "</body></html>";
                    ui->label_status->setText(text);

                    text = "<html><head/><body>";
                    const char* extra_string = "";
                    for(i = 0 ;i < ei.errorString->lines ;i++){
            //            text += "<br/>";
                        if(ei.errorString->mediaInfo){
                            if(i == ei.errorString->lines -2)
                                extra_string = ei.paperSizeString;
                            else if(i == ei.errorString->lines - 1)
                                extra_string = ei.paperTypeString;
                        }
						QString PaperTrayString ="";
                        switch (i) {
                        case 0:
                            if((status.ErrorCodeID == 73 && status.ErrorCodeGroup == 5 )||(status.ErrorCodeID == 72 && status.ErrorCodeGroup == 5 )||currStatus == PS_ERROR_OUT_OF_PAPER|| currStatus == PS_ERROR_PAPER_PROBLEM ||currStatus == PS_ERROR_TRAY_DETACHED)
		                    {
                                PaperTrayString = QString().sprintf(ei.errorString->line0,ei.paperTrayString);
                                text += QString() + "<p>" + PaperTrayString + "</p>";
							}
							else
							{
                                text += QString() + "<p>" + ei.errorString->line0 + extra_string + "</p>";
							}
                            break;
                        case 1:
                            text += QString() + "<p>" + ei.errorString->line1 + extra_string + "</p>";
                            break;
                        case 2:
                            text += QString() + "<p>" + ei.errorString->line2 + extra_string + "</p>";
                            break;
                        case 3:
                            text += QString() + "<p>" + ei.errorString->line3 + extra_string + "</p>";
                            break;
                        case 4:
                            text += QString() + "<p>" + ei.errorString->line4 + extra_string + "</p>";
                            break;
                        default:
                            break;
                        }
                    }
                    text += "</body></html>";
                    ui->label_detail->setText(text);
                }

            }
            else
            {
                text += QString() + "<p>" + get_Status_string(status) + "</p>";
        //        text += QString() + "<p><img src=\"" +status_icon + "\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + get_Status_string(status) + "</p>";
                text += "</body></html>";
                ui->label_status->setText(text);
                ui->label_detail->setText("");
            }
        }

}

void MainWindow::updatePrinter(const QVariant& data)
{
#if 0
    printerInfos = data.value<QList<PrinterInfo_struct> >();
    PrinterInfo_struct printerInfo;
    int base = 0;
    ui->tableWidget_printers->setRowCount(printerInfos.length());
    printers.clear();
    int index_of_defaultprinter = 0;
    for(int i = 0 ;i < printerInfos.length() ;i++){
        printerInfo = printerInfos.at(i);
        if(printerInfo.printer.isDefault){
            index_of_defaultprinter =  i;
        }
        printers << printerInfo.printer.name;
        QTableWidgetItem* item;
        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(printerInfo.printer.name)));
        ui->tableWidget_printers->setItem(i ,base+0 ,item);

//        item = new QTableWidgetItem(tr("%1").arg(get_Status_string(printerInfo.status)));
//        ui->tableWidget_printers->setItem(i ,base+1,item);
        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(printerInfo.printer.makeAndModel)));
        ui->tableWidget_printers->setItem(i ,base+2,item);
        item = new QTableWidgetItem(tr("%1").arg(printerInfo.printer.connectTo));
        ui->tableWidget_printers->setItem(i ,base+3,item);
//        gUInterface->setCmd(UIConfig::CMD_GetStatus ,printerInfo.printer.name);
    }
#else
    printerlist = data.value<QList<Printer_struct> >();
    Printer_struct printer;
    int base = 0;
    ui->tableWidget_printers->setRowCount(printerlist.length());
    printers.clear();
    int index_of_online_printer = -1;
    int index_of_defaultprinter = 0;
    for(int i = 0 ;i < printerlist.length() ;i++){
        printer = printerlist.at(i);
        if(printer.isConnected){
            if(index_of_online_printer < 0)
                index_of_online_printer = i;
        }
        if(printer.isDefault){
            index_of_defaultprinter =  i;
        }
        printers << printer.name;
        QTableWidgetItem* item;
        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(printer.name)));
        ui->tableWidget_printers->setItem(i ,base+0 ,item);

//        item = new QTableWidgetItem(tr("%1").arg(get_Status_string(printerInfo.status)));
//        ui->tableWidget_printers->setItem(i ,base+1,item);
        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(printer.makeAndModel)));
        ui->tableWidget_printers->setItem(i ,base+2,item);
        item = new QTableWidgetItem(tr("%1").arg(printer.connectTo));
        ui->tableWidget_printers->setItem(i ,base+3,item);
//        gUInterface->setCmd(UIConfig::CMD_GetStatus ,printer.name);
    }
#endif

    ui->tableWidget_printers->setColumnHidden(1 ,true);
    if(printers.isEmpty())
    {

        LOGLOG("no printers");
        setcurrentPrinter(QString());
//        gUInterface->setTimer(0);      
        if(bCheckPrinter){
            hide();
            QMessageBox message_box;
            message_box.setButtonText(QMessageBox::Ok ,"确定");
            message_box.setIcon(QMessageBox::Critical);
            message_box.setText("系统中未安装打印机。");
            message_box.setWindowTitle(" ");
            message_box.exec();
            exit(0);
        }
        return;
    }
    else if(printers.contains(current_printer))
    {
//        setcurrentPrinter(current_printer);
         bCheckPrinter = false;
    }else if(index_of_online_printer >= 0){
        bCheckPrinter = false;
        setcurrentPrinter(printers.at(index_of_online_printer));
    }else{
         bCheckPrinter = false;
          setcurrentPrinter(printers.at(index_of_defaultprinter));
    }
    gUInterface->setCurrentPrinterCmd(UIConfig::CMD_GetStatus);
}

void MainWindow::updateOtherStatus(const QString& printer ,const PrinterStatus_struct& status)
{
#if 0
    int base = 0;
    PrinterInfo_struct printerInfo;
    for(int i = 0 ;i < printerInfos.length() ;i++){
        printerInfo = printerInfos.at(i);
        if(printer.compare(printerInfo.printer.name)){
            continue;
        }
        printerInfo.status = status;
        QTableWidgetItem* item;
//        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(printerInfo.printer.name)));
//        ui->tableWidget_printers->setItem(i ,base+0 ,item);
        item = new QTableWidgetItem(tr("%1").arg(get_Status_string(status)));
        ui->tableWidget_printers->setItem(i ,base+1,item);
//        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(printerInfo.printer.makeAndModel)));
//        ui->tableWidget_printers->setItem(i ,base+2,item);
//        item = new QTableWidgetItem(tr("%1").arg(get_connect_to(printerInfo.printer.deviceUri)));
//        ui->tableWidget_printers->setItem(i ,base+3,item);
    }
#else
    int base = 0;
    Printer_struct ps;
    for(int i = 0 ;i < printerlist.length() ;i++){
        ps = printerlist.at(i);
        if(printer.compare(ps.name)){
            continue;
        }
        QTableWidgetItem* item;
//        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(ps.name)));
//        ui->tableWidget_printers->setItem(i ,base+0 ,item);
        item = new QTableWidgetItem(tr("%1").arg(get_Status_string(status)));
        ui->tableWidget_printers->setItem(i ,base+1,item);
//        item = new QTableWidgetItem(tr("%1").arg(QString::fromLocal8Bit(ps.makeAndModel)));
//        ui->tableWidget_printers->setItem(i ,base+2,item);
//        item = new QTableWidgetItem(tr("%1").arg(get_connect_to(ps.deviceUri)));
//        ui->tableWidget_printers->setItem(i ,base+3,item);
    }
#endif
}

#include "tomcat.h"
void MainWindow::updateJobHistory(const QVariant& data)
{
    int base = 0;
    Jobs_struct jobs = data.value<Jobs_struct>();
    jobs_page = jobs.current_page;
    if(jobs_page == 0)
        ui->btn_prepage->setEnabled(false);
    else
        ui->btn_prepage->setEnabled(true);
    if(jobs_page < jobs.pages - 1){
        ui->btn_nextpage->setEnabled(true);
    }else{
        ui->btn_nextpage->setEnabled(false);
    }

//    ui->tableWidget_jobs->clear();
    QStringList job_history = jobs.job_list;
    ui->tableWidget_jobs->setRowCount(job_history.length());
    QTableWidgetItem* item;
//    item = new QTableWidgetItem(QString::fromUtf8("打印机名称"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(0, item);
//    item = new QTableWidgetItem(QString::fromUtf8("计算机名称"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(1, item);
//    item = new QTableWidgetItem(QString::fromUtf8("用户名称"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(2, item);
//    item = new QTableWidgetItem(QString::fromUtf8("文件名称"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(3, item);
//    item = new QTableWidgetItem(QString::fromUtf8("页数"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(4, item);
//    item = new QTableWidgetItem(QString::fromUtf8("份数"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(5, item);
//    item = new QTableWidgetItem(QString::fromUtf8("打印时间"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(6, item);
//    item = new QTableWidgetItem(QString::fromUtf8("是否通过指纹验证"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(7, item);
//    item = new QTableWidgetItem(QString::fromUtf8("验证结果"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(8, item);
//    item = new QTableWidgetItem(QString::fromUtf8("打印结果"));
//    ui->tableWidget_jobs->setHorizontalHeaderItem(9, item);

    base = 0;
    for(int i = 0 ;i < job_history.length() ;i++){
        QString str = job_history.at(i);
        QStringList columns = str.split("/");
        if(columns.length() < 11)
            break;
//        qDebug()<<"columns:"<<columns;
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_printername)));
        ui->tableWidget_jobs->setItem(i ,base+0 ,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_hostname)));
        ui->tableWidget_jobs->setItem(i ,base+1,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_username)));
        ui->tableWidget_jobs->setItem(i ,base+2,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_filename)));
        ui->tableWidget_jobs->setItem(i ,base+3,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_pages)));
        ui->tableWidget_jobs->setItem(i ,base+4,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_copies)));
        ui->tableWidget_jobs->setItem(i ,base+5,item);
        QDateTime datetime;
        datetime.setTime_t(QString(columns.at(JobHistoryIndex_time)).toLong());
        item = new QTableWidgetItem(tr("%1").arg(datetime.toString()));
        ui->tableWidget_jobs->setItem(i ,base+6,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_isFingerEnable)==QString("1")?"是":"否"));
        ui->tableWidget_jobs->setItem(i ,base+7,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_isFingerChecked)==QString("1")?"是":"否"));
        ui->tableWidget_jobs->setItem(i ,base+8,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(JobHistoryIndex_isJobCompleted)==QString("1")?"完成":"未完成"));
        ui->tableWidget_jobs->setItem(i ,base+9,item);
    }
}


QString changePassword(const QString& password = "1234ABCD");
void MainWindow::on_pushButton_changePassword_clicked()
{
    QVariant sys_password;
    appSettings("password" ,sys_password ,QVariant(QString("1234ABCD")));
    QString password = changePassword(sys_password.toString());
    if(password.compare(sys_password.toString())){
        sys_password.setValue(password);
        appSettings("password" ,sys_password ,QVariant(QString("1234ABCD")) ,true);
    }
}

void MainWindow::on_checkBox_record_clicked()
{
    if(!validate_password()){
        QVariant value;
        record_printlist = ui->checkBox_record->isChecked();
        value.setValue(record_printlist);
        appSettings("record" ,value ,QVariant(false) ,true);
    }else{
        ui->checkBox_record->setChecked(record_printlist);
    }
}

void MainWindow::on_btn_prepage_clicked()
{
    if(jobs_page){
        jobs_page--;
        gUInterface->setCurrentPrinterCmd(UIConfig::CMD_GetJobs ,jobs_page);
    }
}

void MainWindow::on_btn_nextpage_clicked()
{
    jobs_page++;
    gUInterface->setCurrentPrinterCmd(UIConfig::CMD_GetJobs ,jobs_page);
}
