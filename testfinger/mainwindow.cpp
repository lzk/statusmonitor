#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "filter_check_finger.h"
#include <QDebug>
#ifndef SERVER_PATH
#ifdef LOONGSON
//#define SERVER_PATH "/var/run/cups/tjgd1zsm.domain"
#define SERVER_PATH "/var/spool/cups/tmp/tjgd1zsm.domain"
#else
#define SERVER_PATH "/tmp/tjgd1zsm.domain"
#endif
#endif
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBox->setValue(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "jkinterface.h"
void MainWindow::on_pushButton_clicked()
{
//    int ret = checkFinger(ui->spinBox->value());
//    qDebug()<<"check finger :"<<ret;

    Trans_Client tc(SERVER_PATH);
    char buffer[256];
    sprintf(buffer ,"start://%s?jobid=%d" ,"test_printer" ,ui->spinBox->value());
    tc.writeThenRead(buffer ,sizeof(buffer));
    if(!strcmp(buffer ,"startok")){
        LOGLOG("test ok");
    }

//    sprintf(buffer ,"result://%s?jobid=%d&status=%d&username=%s&filename=%s","printer" ,ui->spinBox->value(), 1
//            ,"username" ,"filename");
//    tc.writeThenRead(buffer ,sizeof(buffer));
}
