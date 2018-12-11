#include "ipv6status.h"
#include "ui_ipv6status.h"

IPv6Status::IPv6Status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPv6Status)
{
    ui->setupUi(this);
}

IPv6Status::~IPv6Status()
{
    delete ui;
}
