#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "jkinterface.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LOGLOG("test!!!");
}

MainWindow::~MainWindow()
{
    delete ui;
}
