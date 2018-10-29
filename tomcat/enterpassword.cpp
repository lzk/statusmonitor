#include "enterpassword.h"
#include "ui_enterpassword.h"

EnterPassword::EnterPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterPassword)
{
    ui->setupUi(this);
}

EnterPassword::~EnterPassword()
{
    delete ui;
}

QString EnterPassword::getPassword()
{
    return ui->lineEdit->text();
}

QString getEnterPassword()
{
    int result;
    EnterPassword* dialog = new EnterPassword;
    dialog->setWindowTitle("输入密码");
    result = dialog->exec();
    if(QDialog::Accepted == result){
        return dialog->getPassword();
    }
    return QString();
}
