#include "enterpassword.h"
#include "ui_enterpassword.h"
#include <QPushButton>
EnterPassword::EnterPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterPassword)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");

//    QRegExp regexp;
//    regexp.setPattern("^?[0-9a-fA-F]{8,16}$");
    QRegExp regexp("^[\\x0020-\\x007e]{8,16}$");
    QValidator *validator = new QRegExpValidator(regexp, this);
    ui->lineEdit->setValidator(validator);

}

EnterPassword::~EnterPassword()
{
    delete ui;
}

QString EnterPassword::getPassword()
{
    return ui->lineEdit->text();
}

int getEnterPassword(QString& password)
{
    int result;
    EnterPassword* dialog = new EnterPassword;
    dialog->setWindowTitle("输入密码");
    result = dialog->exec();
    if(QDialog::Accepted == result){
        password = dialog->getPassword();
        result = 0;
    }else{
        result = -1;
    }
    return result;
}
