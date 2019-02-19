#include "about.h"
#include "ui_about.h"

extern const char* app_version;
About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowTitle(" ");
    ui->label_version->setText(QString("TOEC状态监视器版本%1").arg(app_version));

}

About::~About()
{
    delete ui;
}
