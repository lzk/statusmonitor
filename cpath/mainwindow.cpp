#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "jkinterface.h"
#include "membercenter/about.h"
#include "promptdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LOGLOG("test!!!");
    ui->memberCenterWidget->setSW(ui->totalStackedWidget, ui->loginButton);

    selectState = "background-color: rgb(99, 99, 99);color:white;";
    unSelectState = "background-color: white;color:black;";

}

MainWindow::~MainWindow()
{
    delete ui;
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
        pDialog->setDialogMsg("应用关闭后，扫描图片将会被删除，是否关闭，请确认。");
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
