#include "settingforipv6.h"
#include "ui_settingforipv6.h"

SettingForIPv6::SettingForIPv6(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingForIPv6)
{
    ui->setupUi(this);
}

SettingForIPv6::~SettingForIPv6()
{
    delete ui;
}
