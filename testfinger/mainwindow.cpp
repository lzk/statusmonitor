#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filter_check_finger.h"
#include <QDebug>
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

void MainWindow::on_pushButton_clicked()
{
    int ret = checkFinger(ui->spinBox->value());
    qDebug()<<"check finger :"<<ret;
}
