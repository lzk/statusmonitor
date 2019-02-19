#include "changepassword.h"
#include "ui_changepassword.h"
#include <QPushButton>
#include <QDebug>
ChangePassword::ChangePassword(const QString& _password ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassword),
    password(_password)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->lineEdit_new ,SIGNAL(textEdited(QString)) ,this ,SLOT(on_lineEdit_old_textEdited(QString)));
    connect(ui->lineEdit_confirm ,SIGNAL(textEdited(QString)) ,this ,SLOT(on_lineEdit_old_textEdited(QString)));
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

QString ChangePassword::getPassword()
{
    return ui->lineEdit_new->text();
}

QString changePassword(const QString& password)
{
    QString pd = password;
    int result;
    ChangePassword* dialog = new ChangePassword(password);
    dialog->setWindowTitle("更改密码");
    result = dialog->exec();
    if(QDialog::Accepted == result){
        pd = dialog->getPassword();
    }
    return pd;
}

void ChangePassword::on_lineEdit_old_textEdited(const QString &)
{
    if(!ui->lineEdit_old->text().compare(password)
            && !ui->lineEdit_new->text().isEmpty()
            && ui->lineEdit_new->text().compare(password)
            && !ui->lineEdit_new->text().compare(ui->lineEdit_confirm->text())){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }else{
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
