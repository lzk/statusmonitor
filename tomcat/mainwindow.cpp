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
//#include <QMessageBox>
#include "commonapi.h"
#define TEST 0
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(app_name);
//    setWindowIcon(QIcon(":/image/app_icon.png"));

    this->setFixedSize(500 ,380);
    ui->statusUpdate_groupBox->hide();

    createSysTray();

    connect(gUInterface ,SIGNAL(cmdResult(int,int,QVariant)) ,this ,SLOT(cmdResult(int,int,QVariant)));

    gUInterface->setCmd(UIConfig::CMD_GetPrinters ,QString());
//    gUInterface->setCmd(UIConfig::CMD_GetDefaultPrinter);
//    gUInterface->setTimer(6);

    QVariant value;
    appSettings("record" ,value ,QVariant(false));
    record_printlist = value.toBool();
    ui->checkBox_record->setChecked(record_printlist);
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

void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (trayIcon->isVisible()) {
//        QMessageBox::information(this, tr("Systray"),
//                                 tr("The program will keep running in the "
//                                    "system tray. To terminate the program, "
//                                    "choose <b>Quit</b> in the context menu "
//                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

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
    gUInterface->setCmd(UIConfig::CMD_GetStatus ,current_printer);
}

void MainWindow::on_checkBox_clicked()
{

    if(ui->checkBox->isChecked()){
        gUInterface->setTimer(ui->lineEdit_updateInterval->text().toInt());
    }else{
        gUInterface->setTimer();
    }
}

QString getEnterPassword();
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
        QString password = getEnterPassword();
        QVariant sys_password;
        appSettings("password" ,sys_password ,QVariant(QString("1234ABCD")));
        if(!password.compare(sys_password.toString())){
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
        item = new QTableWidgetItem(tr("%1").arg(QString("节电中")));
        ui->tableWidget_printers->setItem(0 ,1,item);
        item = new QTableWidgetItem(tr("%1").arg(QString("TOEC Printer")));
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
    }else if(c < 5){
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
    }else if(m < 5){
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
    }else if(y < 5){
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
    }else if(k < 5){
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

    ui->textEdit_toner->setText(toner_text);

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
        text += QString() + "<p><img src=\":/image/status_normal.png\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + get_Status_string(status) + "</p>";
    }else{
        ErrorInfo_struct ei = getErrorInfo(status.ErrorCodeGroup ,status.ErrorCodeID ,status.PaperType ,status.PaperSize);
        if(!ei.error || !ei.errorString){
            text += QString() + "<p><img src=\":/image/status_normal.png\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + get_Status_string(status) + "</p>";
            goto CODE_ERROR;
        }

        text += QString() + "<p><img src=\":/image/status_normal.png\"/>&nbsp;&nbsp;&nbsp;&nbsp;" + ei.errorString->title + "</p>";
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
    ui->tableWidget_printers->setColumnHidden(1 ,true);
    if(printers.isEmpty()){
        LOGLOG("no printers");
        setcurrentPrinter(QString());
//        gUInterface->setTimer(0);
        return;
    }else if(printers.contains(current_printer)){
//        setcurrentPrinter(current_printer);
    }else{
        setcurrentPrinter(printers.at(index_of_defaultprinter));
    }
}

void MainWindow::updateOtherStatus(const QString& printer ,const PrinterStatus_struct& status)
{
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

}

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
    QStringList job_history = jobs.job_list;
    ui->tableWidget_jobs->setRowCount(job_history.length());
    base = 0;
    for(int i = 0 ;i < job_history.length() ;i++){
        QString str = job_history.at(i);
        QStringList columns = str.split(",");
        if(columns.length() < 11)
            break;
        qDebug()<<"columns:"<<columns;
        QTableWidgetItem* item;
        item = new QTableWidgetItem(tr("%1").arg(columns.at(1)));
        ui->tableWidget_jobs->setItem(i ,base+0 ,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(2)));
        ui->tableWidget_jobs->setItem(i ,base+1,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(3)));
        ui->tableWidget_jobs->setItem(i ,base+2,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(4)));
        ui->tableWidget_jobs->setItem(i ,base+3,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(5)));
        ui->tableWidget_jobs->setItem(i ,base+4,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(6)));
        ui->tableWidget_jobs->setItem(i ,base+5,item);
        QDateTime datetime;
        datetime.setTime_t(QString(columns.at(7)).toLong());
        item = new QTableWidgetItem(tr("%1").arg(datetime.toString()));
        ui->tableWidget_jobs->setItem(i ,base+6,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(8)==QString("1")?"是":"否"));
        ui->tableWidget_jobs->setItem(i ,base+7,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(9)==QString("1")?"成功":"失败"));
        ui->tableWidget_jobs->setItem(i ,base+8,item);
        item = new QTableWidgetItem(tr("%1").arg(columns.at(10)==QString("1")?"是":"否"));
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
    QString password = getEnterPassword();
    QVariant sys_password;
    appSettings("password" ,sys_password ,QVariant(QString("1234ABCD")));
    if(!password.compare(sys_password.toString())){
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
