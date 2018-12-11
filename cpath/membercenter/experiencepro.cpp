#include "experiencepro.h"
#include "ui_experiencepro.h"

ExperiencePro::ExperiencePro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperiencePro)
{
    ui->setupUi(this);
    ui->lab_msg->setWordWrap(true);
    ui->lab_msg->setText(QString::fromLocal8Bit("为了帮助我们持续提供更好版本的“小新打印”和更好的用户服务，Lenovo希望定期收集用户数据，如注册会员手机号、打印文件类型与频率、打印机型号及相关信息、手持设备位置与标识、PC标识等"));
    ui->lab_check_info->setWordWrap(true);
    ui->lab_check_info->setText(tr("我已阅读并同意Lenovo在线隐私声明，并允许自动数据收集"));
}

ExperiencePro::~ExperiencePro()
{
    delete ui;
}

void ExperiencePro::on_bt_OK_clicked()
{
    close();
}
