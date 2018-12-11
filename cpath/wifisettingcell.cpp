#include "wifisettingcell.h"
#include "ui_wifisettingcell.h"

WiFiSettingCell::WiFiSettingCell(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::WiFiSettingCell)
{
    ui->setupUi(this);
}

WiFiSettingCell::~WiFiSettingCell()
{
    delete ui;
}
