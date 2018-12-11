#include "settingsstackedwidget.h"
#include "ui_settingsstackedwidget.h"

SettingsStackedWidget::SettingsStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::SettingsStackedWidget)
{
    ui->setupUi(this);
    this->setCurrentIndex(0);
    cycle = new BusyRefreshLabel(parent);
    cycle->setGeometry(QRect(585,400,50,50));
    scrollArea = new QScrollArea(ui->WiFi);
    scrollArea->setGeometry(QRect(13, 14, 235, 274));
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setStyleSheet("QScrollBar:vertical"
                            "{"
                            "width:10px;"
                            "background:rgba(0,0,0,0%);"
                            "margin:0px,0px,0px,0px;"
                            "padding-top:12px;"
                            "padding-bottom:12px;"
                            "}"
                            "QScrollBar::handle:vertical"
                            "{"
                            "width:8px;"
                            "background:rgba(0,0,0,25%);"
                            " border-radius:5px;"
                            "min-height:20;"
                            "}"
                            "QScrollBar::handle:vertical:hover"
                            "{"
                            "width:8px;"
                            "background:rgba(0,0,0,50%);"
                            " border-radius:5px;"
                            "min-height:20;"
                            "}"
                            "QScrollBar::add-line:vertical"
                            "{"
                            "height:12px;width:12px;"
                            "subcontrol-position:bottom;"
                            "}"
                            "QScrollBar::sub-line:vertical"
                            "{"
                            "height:12px;width:12px;"
                            "subcontrol-position:top;"
                            "}"
                            "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                            "{"
                            "background:rgba(0,0,0,0%);"
                            "border-radius:4px;"
                            "}"
                            );
    isLogn = false;     //isLogn :default false; false:did not comfirm the pw in the device;
    titelCell = new WlanTitleCell(scrollArea, true, &isLogn);
    titelCell->cycle = cycle;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(titelCell);
    timer1 = new QTimer;
    //initAP();
    //initIP();
    //initPowerSave();
    //initPassword();
   // initAdvanceSetting(userconfig, true);
    //connect(device ,SIGNAL(signals_cmd_result(int,int)), this ,SLOT(slots_cmd_result(int,int)));

    isDoingCMD = false;
    retryTimes = 0;
}

SettingsStackedWidget::~SettingsStackedWidget()
{
    delete ui;
}

void SettingsStackedWidget::changeStackIndex(int index)
{
    switch(index)
    {
    case 0:                                     //wifi setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            //titelCell->setCurrentIndex(0);
            //device->cmd_wifi_refresh();
            this->setCurrentIndex(index);
            //cycle->startAnimation(20);
        }
        break;
    case 1:                                     //softAP setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            //device->cmd_wifi_get_softap();
            this->setCurrentIndex(index);
            //cycle->startAnimation(20);
        }
        break;
    case 2:                                    // ip settting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
           // device->cmd_net_get_ipv4();
            this->setCurrentIndex(index);
            //cycle->startAnimation(20);
            ui->btIPv4->setEnabled(false);
            ui->btIPv6->setEnabled(false);
        }
        break;
    case 3:                                    // power save time setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            //device->cmd_printer_get_saveTime();
            this->setCurrentIndex(index);
            //cycle->startAnimation(20);
        }
        break;
    case 4:                                   // advance setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
           // device->cmd_printer_get_userConfig();
            this->setCurrentIndex(index);
            //cycle->startAnimation(20);
        }
        break;
    case 5:                                   //new password setting
        //if(CMD_STATUS_COMPLETE == device->get_cmdStatus())
        {
            this->setCurrentIndex(index);
        }
        break;
    default:
        break;
    }
}

void SettingsStackedWidget::initAP()
{
   // if(NULL != device)
    {
       // device->
    }
    isAPOpen = true;
    ui->btAPOpen->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
    ui->lineEdit_SSID_AP->setText(tr(""));
    ui->lineEdit_SSID_AP->setCursorPosition(1);
    ui->lineEdit_Password_AP->clear();

    QRegExp rx1("^[0-9a-zA-Z]{1,32}$");
    QRegExp rx2("^[\\x0020-\\x007e]{8,64}$");
    QRegExpValidator *validator1 = new QRegExpValidator(rx1, this);
    QRegExpValidator *validator2 = new QRegExpValidator(rx2, this);
    ui->lineEdit_SSID_AP->setValidator(validator1);
    ui->lineEdit_Password_AP->setValidator(validator2);
}

void SettingsStackedWidget::initIP()
{
    ui->label_ipaddr_error->hide();
    ui->label_gateway_error->hide();
    ui->label_sumask_error->hide();
    ui->lineEdit_IPAddressv4->installEventFilter(this);
    ui->lineEdit_Gatewayv4->installEventFilter(this);
    ui->lineEdit_Submaskv4->installEventFilter(this);

    /*if(0 == info_ipv4.SubnetMask[0])            //if true, that means cmd_get_ipv4 did not success;
    {
        info_ipv4.IPAddressMode = 3;            //to set default value;
        info_ipv4.IPMode = 1;
    }*/
//    if(0==info_ipv4.IPMode)                   /*****IPV4 mode *******/
//    {
       /* on_btIPv4_clicked();
        if(info_ipv4.IPAddressMode < 4)         //DHCP mode
        {
             ui->btDHCP->setChecked(true);
             on_btDHCP_toggled(true);
             QString text = "%1.%2.%3.%4";
             text = text.arg(info_ipv4.IPAddress[0]).arg(info_ipv4.IPAddress[1]).\
                                        arg(info_ipv4.IPAddress[2]).arg(info_ipv4.IPAddress[3]);
             ui->lineEdit_IPAddressv4->setText(tr(text.toLatin1()));
             text = "%1.%2.%3.%4";
             text = text.arg(info_ipv4.GatewayAddress[0]).arg(info_ipv4.GatewayAddress[1]).\
                                        arg(info_ipv4.GatewayAddress[2]).arg(info_ipv4.GatewayAddress[3]);
             ui->lineEdit_Gatewayv4->setText(tr(text.toLatin1()));
             text = "%1.%2.%3.%4";
             text = text.arg(info_ipv4.SubnetMask[0]).arg(info_ipv4.SubnetMask[1]).\
                                        arg(info_ipv4.SubnetMask[2]).arg(info_ipv4.SubnetMask[3]);
             ui->lineEdit_Submaskv4->setText(tr(text.toLatin1()));
        }
        else if(4 == info_ipv4.IPAddressMode)       //static ip mode
        {
            ui->btStatic->setChecked(true);
            on_btStatic_toggled(true);
            QString text = "%1.%2.%3.%4";
            text = text.arg(info_ipv4.IPAddress[0]).arg(info_ipv4.IPAddress[1]).\
                                       arg(info_ipv4.IPAddress[2]).arg(info_ipv4.IPAddress[3]);
            ui->lineEdit_IPAddressv4->setText(tr(text.toLatin1()));
            text = "%1.%2.%3.%4";
            text = text.arg(info_ipv4.GatewayAddress[0]).arg(info_ipv4.GatewayAddress[1]).\
                                       arg(info_ipv4.GatewayAddress[2]).arg(info_ipv4.GatewayAddress[3]);
            ui->lineEdit_Gatewayv4->setText(tr(text.toLatin1()));
            text = "%1.%2.%3.%4";
            text = text.arg(info_ipv4.SubnetMask[0]).arg(info_ipv4.SubnetMask[1]).\
                                       arg(info_ipv4.SubnetMask[2]).arg(info_ipv4.SubnetMask[3]);
            ui->lineEdit_Submaskv4->setText(tr(text.toLatin1()));
        }*/
//    }
}

void SettingsStackedWidget::initPowerSave()
{
    ui->lineEdit_timeout->setText("1");
    QRegExp rx4("^[0-9]{0,3}$");
    QRegExpValidator *validator4 = new QRegExpValidator(rx4, this);
    ui->lineEdit_timeout->setValidator(validator4);
    ui->lineEdit_timeout->installEventFilter(this);

    ui->label_timeout_error->hide();

    connect(timer1, SIGNAL(timeout()), this, SLOT(on_timer1_timeout())); //this timer work for add  & reduce button;
}

void SettingsStackedWidget::initPassword()
{
    QRegExp rx4("^[\\x0020-\\x0076]{0,32}$");
    QRegExpValidator *validator4 = new QRegExpValidator(rx4, this);
    ui->lineEdit_newPassWord->setValidator(validator4);
    ui->lineEdit_confirm->setValidator(validator4);
}

/*void SettingsStackedWidget::initAdvanceSetting(cmdst_userconfig config, bool isFirst)
{
    if(isFirst)             //if ture, set the default value;
    {
        QRegExp rx1("-{0,1}[0-9]{1,2}");
        QRegExpValidator *validator1 = new QRegExpValidator(rx1,this);
        ui->lineEdit_TopMargin->setValidator(validator1);
        ui->lineEdit_LeftMargin->setValidator(validator1);
        ui->lineEdit_ImageDensity->setValidator(validator1);

        ui->lineEdit_TopMargin->setText("0");
        ui->lineEdit_LeftMargin->setText("0");
        ui->lineEdit_ImageDensity->setText("0");
        ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        isLowHumidity = false;
        ui->btPlateCtrl->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        isConrtolM = false;
        ui->btCoolMode->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
        isCoolingM = false;

        ui->label_setting_error->hide();
        ui->lineEdit_TopMargin->installEventFilter(this);
        ui->lineEdit_LeftMargin->installEventFilter(this);
        ui->lineEdit_ImageDensity->installEventFilter(this);
    }
    else                //if get the value form device, set the value got;
    {
        ui->lineEdit_ImageDensity->setText(QString::number(config.ImageDensity));
        ui->lineEdit_LeftMargin->setText(QString::number(config.SideToSide));
        ui->lineEdit_TopMargin->setText(QString::number(config.LeadingEdge));

        isLowHumidity = (0 == config.LowHumidityMode ? false : true);
        if(isLowHumidity)
            ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        else
            ui->btLowHumidity->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");

        isConrtolM = (0 == config.PlateControlMode ? true : false);
        if(isConrtolM)
            ui->btPlateCtrl->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        else
            ui->btPlateCtrl->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");

        isCoolingM = (0 == config.PrimaryCoolingMode ? false : true);
        if(isCoolingM)
            ui->btCoolMode->setStyleSheet("border-image: url(:/Images/CheckBox_Open.png);");
        else
            ui->btCoolMode->setStyleSheet("border-image: url(:/Images/CheckBox_Close.png);");
    }
}*/
