#include "experiencepro.h"
#include "ui_experiencepro.h"

ExperiencePro::ExperiencePro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperiencePro)
{
    ui->setupUi(this);
    ui->lab_msg->setWordWrap(true);
    ui->lab_msg->setText(tr("ResStr_In_order_to_deliver_improved"));
    ui->lab_check_info->setWordWrap(true);
    ui->lab_check_info->setText(tr("ResStr_I_have_read_and_agree"));
}

ExperiencePro::~ExperiencePro()
{
    delete ui;
}

void ExperiencePro::on_bt_OK_clicked()
{
    close();
}
