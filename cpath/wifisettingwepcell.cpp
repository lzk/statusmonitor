#include "wifisettingwepcell.h"
#include "ui_wifisettingwepcell.h"

WiFiSettingWEPCell::WiFiSettingWEPCell(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::WiFiSettingWEPCell)
{
    ui->setupUi(this);
}

WiFiSettingWEPCell::~WiFiSettingWEPCell()
{
    delete ui;
}
