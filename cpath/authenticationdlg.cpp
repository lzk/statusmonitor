#include "authenticationdlg.h"
#include "ui_authenticationdlg.h"

AuthenticationDlg::AuthenticationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthenticationDlg)
{
    ui->setupUi(this);
}

AuthenticationDlg::~AuthenticationDlg()
{
    delete ui;
}
