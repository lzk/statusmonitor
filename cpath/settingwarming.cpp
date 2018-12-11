#include "settingwarming.h"
#include "ui_settingwarming.h"

SettingWarming::SettingWarming(QWidget *parent, QString text, bool showLabel) :
    QDialog(parent),
    ui(new Ui::SettingWarming)
{
    ui->setupUi(this);
    if(text != NULL)
    {
        ui->label->setText(text);
    }
    else
    {
        ui->label->setText(tr(" The password must be 5/13 ASCII characters \n or 10/26 hex characters, \n please confirm and enter again."));
    }
    if(showLabel)
    {
        ui->label_2->hide();
        ui->label->setGeometry(QRect(60,10,340,90));
    }
}

SettingWarming::~SettingWarming()
{
    delete ui;
}

void SettingWarming::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
