#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this); 
    ui->lab_version->setText(QString("%1 %2").arg(tr("ResStr_Lenovo_Printer")).arg("1.0.0.1"));
}

About::~About()
{
    delete ui;
}
